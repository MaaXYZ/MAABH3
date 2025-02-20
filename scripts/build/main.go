package main

import (
	"flag"
	"fmt"
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"time"
)

var (
	appName    = "mba"
	moduleName = "github.com/MaaXYZ/maa-boom-aid"
	binDir     = "bin"
)

var (
	osList   = []string{"linux", "windows", "darwin"}
	archList = []string{"amd64", "arm64"}
)

func main() {
	var (
		buildAt, goVersion, version, targetOS, targetArch string
		all                                               bool
	)
	flag.StringVar(&buildAt, "buildAt", time.Now().Format(time.RFC3339), "Build time")
	flag.StringVar(&goVersion, "goVersion", runtime.Version(), "Go version used for build")
	flag.StringVar(&version, "version", "dev", "Application version")
	flag.StringVar(&targetOS, "os", runtime.GOOS, "Target operating system")
	flag.StringVar(&targetArch, "arch", runtime.GOARCH, "Target architecture")
	flag.BoolVar(&all, "all", false, "Build all operating system and architecture")
	flag.Parse()

	if err := os.MkdirAll(binDir, 0755); err != nil {
		log.Fatalf("Failed to create bin directory: %v\n", err)
	}
	if all {
		for _, targetOS := range osList {
			for _, targetArch := range archList {
				if err := buildProject(buildAt, goVersion, version, targetOS, targetArch); err != nil {
					log.Fatalf("Failed to build %s: %v\n", appName, err)
				}
			}
		}
	} else {
		if err := buildProject(buildAt, goVersion, version, targetOS, targetArch); err != nil {
			log.Fatalf("Failed to build %s: %v\n", appName, err)
		}
	}
}

func buildProject(buildAt, goVersion, version, targetOS, targetArch string) error {
	xBuildAt := fmt.Sprintf("-X '%s/internal/config.BuildAt=%s'", moduleName, buildAt)
	xGoVersion := fmt.Sprintf("-X '%s/internal/config.GoVersion=%s'", moduleName, goVersion)
	xVersion := fmt.Sprintf("-X '%s/internal/config.Version=%s'", moduleName, version)

	var ldflags string
	if version != "dev" {
		ldflags = fmt.Sprintf("-w -s %s %s %s", xBuildAt, xGoVersion, xVersion)
	} else {
		ldflags = fmt.Sprintf("%s %s %s", xBuildAt, xGoVersion, xVersion)
	}

	log.Printf("Start building for %s on %s/%s.\n", appName, targetOS, targetArch)

	outputPath := filepath.Join(".", binDir, fmt.Sprintf("%s-%s", targetOS, targetArch), appName)
	if targetOS == "windows" {
		outputPath += ".exe"
	}

	buildCommand := []string{
		"build",
		"-o", outputPath,
		"-ldflags", ldflags,
		"main.go",
	}

	cmd := exec.Command("go", buildCommand...)

	cgoDisabled := "CGO_ENABLED=0"
	cmd.Env = append(os.Environ(),
		cgoDisabled,
		fmt.Sprintf("GOOS=%s", targetOS),
		fmt.Sprintf("GOARCH=%s", targetArch),
	)
	cmd.Stderr = os.Stderr
	cmd.Stdout = os.Stdout

	startTime := time.Now()
	if err := cmd.Run(); err != nil {
		return err
	}
	duration := time.Since(startTime)

	log.Printf("Successfully built %s in %s.\n", appName, duration)
	return nil
}
