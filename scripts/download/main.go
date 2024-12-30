package main

import (
	"archive/zip"
	"context"
	"flag"
	"io"
	"log"
	"net/http"
	"os"
	"path/filepath"
	"runtime"
	"strings"

	"github.com/google/go-github/v67/github"
	"golang.org/x/oauth2"
)

const depsDir = "deps"

var osMap = map[string]string{
	"linux":   "linux",
	"windows": "win",
	"darwin":  "macos",
}

var archMap = map[string]string{
	"amd64": "x86_64",
	"arm64": "aarch64",
}

func main() {
	var (
		targetOS, targetArch, version, token string
		all                                  bool
	)
	flag.StringVar(&targetOS, "system", runtime.GOOS, "Target system (e.g., linux, windows, darwin)")
	flag.StringVar(&targetArch, "arch", runtime.GOARCH, "Target architecture (e.g., amd64, arm64)")
	flag.StringVar(&version, "version", "latest", "Version to download (e.g., v2.2.2)")
	flag.StringVar(&token, "token", "", "GitHub API token (optional)")
	flag.BoolVar(&all, "all", false, "Download all assets")
	flag.Parse()

	ctx := context.Background()
	var client *github.Client

	if token != "" {
		ts := oauth2.StaticTokenSource(
			&oauth2.Token{AccessToken: token},
		)
		tc := oauth2.NewClient(ctx, ts)
		client = github.NewClient(tc)
	} else {
		client = github.NewClient(nil)
	}

	var release *github.RepositoryRelease
	if version == "latest" {
		releases, _, err := client.Repositories.ListReleases(ctx, "MaaXYZ", "MaaFramework", nil)
		if err != nil {
			log.Fatalf("Error fetching releases: %v", err)
		}
		if len(releases) == 0 {
			log.Fatal("No releases found")
		}
		release = releases[0]
	} else {
		rel, _, err := client.Repositories.GetReleaseByTag(ctx, "MaaXYZ", "MaaFramework", version)
		if err != nil {
			log.Fatalf("Error fetching release: %v", err)
		}
		release = rel
	}

	log.Printf("Selected release: %s", *release.TagName)

	for _, asset := range release.Assets {
		name := asset.GetName()
		var extractPath string
		if all {
			for osName, osValue := range osMap {
				for archName, archValue := range archMap {
					if strings.Contains(name, osValue) && strings.Contains(name, archValue) {
						extractPath = filepath.Join(depsDir, osName+"-"+archName)
						break
					}
				}
			}
		} else {
			downloadOs := osMap[targetOS]
			downloadArch := archMap[targetArch]
			if strings.Contains(name, downloadOs) && strings.Contains(name, downloadArch) {
				extractPath = filepath.Join(depsDir, targetOS+"-"+targetArch)
			}
		}

		if extractPath != "" {
			dest := filepath.Join(depsDir, name)
			if _, err := os.Stat(dest); os.IsNotExist(err) {
				log.Printf("Downloading: %s", name)
				err := downloadFile(asset.GetBrowserDownloadURL(), dest)
				if err != nil {
					log.Printf("Error downloading file: %v", err)
					continue
				}
			}

			log.Printf("Extracting to: %s", extractPath)
			err := unzip(dest, extractPath)
			if err != nil {
				log.Printf("Error extracting file: %v", err)
			} else {
				log.Printf("Successfully processed: %s", name)
			}
		}
	}
}

func downloadFile(url, dest string) error {
	resp, err := http.Get(url)
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	out, err := os.Create(dest)
	if err != nil {
		return err
	}
	defer out.Close()

	_, err = io.Copy(out, resp.Body)
	return err
}

func unzip(src, dest string) error {
	r, err := zip.OpenReader(src)
	if err != nil {
		return err
	}
	defer r.Close()

	for _, f := range r.File {
		rc, err := f.Open()
		if err != nil {
			return err
		}
		defer rc.Close()

		path := filepath.Join(dest, f.Name)
		if f.FileInfo().IsDir() {
			os.MkdirAll(path, f.Mode())
		} else {
			os.MkdirAll(filepath.Dir(path), os.ModePerm)
			out, err := os.OpenFile(path, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, f.Mode())
			if err != nil {
				return err
			}
			defer out.Close()

			_, err = io.Copy(out, rc)
			if err != nil {
				return err
			}
		}
	}
	return nil
}
