package main

import (
	"flag"
	"fmt"
	"io"
	"log"
	"os"
	"path/filepath"
	"runtime"
)

const (
	binaryNameBase = "mba"
	binDirBase     = "bin"
	installDirBase = "install"
	depsDirBase    = "deps"
	assetsDir      = "assets"
	piCliBase      = "MaaPiCli"
)

var (
	osList   = []string{"linux", "windows", "darwin"}
	archList = []string{"amd64", "arm64"}
)

func main() {
	var (
		targetOS, targetArch string
		clearInstall, all    bool
	)

	flag.StringVar(&targetOS, "os", runtime.GOOS, "Target operating system")
	flag.StringVar(&targetArch, "arch", runtime.GOARCH, "Target architecture")
	flag.BoolVar(&clearInstall, "clear", false, "Clear install directory before installation")
	flag.BoolVar(&all, "all", false, "Intall all operating system and architecture")
	flag.Parse()

	if all {
		for _, targetOS := range osList {
			for _, targetArch := range archList {
				install(targetOS, targetArch, clearInstall)
			}
		}
	} else {
		install(targetOS, targetArch, clearInstall)
	}

}

func install(targetOS, targetArch string, clearInstall bool) {
	osArch := formatOsArch(targetOS, targetArch)
	binDir := filepath.Join(binDirBase, osArch)
	installDir := filepath.Join(installDirBase, osArch)

	validateDirectoryExists(binDir)

	if clearInstall {
		removeDirectory(installDir)
	}

	createDirectory(installDir)

	binaryName := getBinaryName(targetOS)
	if err := copyFile(filepath.Join(binDir, binaryName), filepath.Join(installDir, binaryName)); err != nil {
		log.Fatalf("Failed to copy binary file %s: %v", binaryName, err)
	}

	installDependencies(targetOS, targetArch, installDir)

	log.Println("Install completed successfully.")
}

func formatOsArch(os, arch string) string {
	return fmt.Sprintf("%s-%s", os, arch)
}

func getBinaryName(targetOS string) string {
	if targetOS == "windows" {
		return binaryNameBase + ".exe"
	}
	return binaryNameBase
}

func validateDirectoryExists(dir string) {
	if _, err := os.Stat(dir); os.IsNotExist(err) {
		log.Fatalf("Error: directory %s does not exist", dir)
	}
}

func removeDirectory(dir string) {
	log.Println("Clearing install directory...")
	if err := os.RemoveAll(dir); err != nil {
		log.Fatalf("Failed to clear install directory: %v", err)
	}
}

func createDirectory(dir string) {
	if err := os.MkdirAll(dir, 0755); err != nil {
		log.Fatalf("Failed to create directory: %v", err)
	}
}

func installDependencies(targetOS, targetArch, installDir string) {
	depsDir := filepath.Join(depsDirBase, formatOsArch(targetOS, targetArch))
	depsBinDir := filepath.Join(depsDir, "bin")

	if err := copyDir(depsBinDir, installDir); err != nil {
		log.Fatalf("Failed to copy deps binary directory: %v", err)
	}

	removeFileIfExists(filepath.Join(installDir, getPiCliName(targetOS)))

	maaAgentBinaryDir := filepath.Join(depsDir, "share", "MaaAgentBinary")
	if err := copyDir(maaAgentBinaryDir, filepath.Join(installDir, "MaaAgentBinary")); err != nil {
		log.Fatalf("Failed to copy MaaAgentBinary directory %s: %v", maaAgentBinaryDir, err)
	}

	resDir := filepath.Join(assetsDir, "resource")
	if err := copyDir(resDir, filepath.Join(installDir, "resource")); err != nil {
		log.Fatalf("Failed to copy resource directory %s: %v", resDir, err)
	}

	createDirectory(filepath.Join(installDir, "config"))
}

func getPiCliName(targetOS string) string {
	if targetOS == "windows" {
		return piCliBase + ".exe"
	}
	return piCliBase
}

func removeFileIfExists(filePath string) {
	if err := os.Remove(filePath); err != nil && !os.IsNotExist(err) {
		log.Fatalf("Failed to remove %s: %v", filePath, err)
	}
}

func copyDir(srcDir, dstDir string) error {
	return filepath.Walk(srcDir, func(path string, info os.FileInfo, err error) error {
		if err != nil {
			return err
		}

		relPath, err := filepath.Rel(srcDir, path)
		if err != nil {
			return err
		}
		dstPath := filepath.Join(dstDir, relPath)

		if info.IsDir() {
			return os.MkdirAll(dstPath, info.Mode())
		}
		return copyFile(path, dstPath)
	})
}

func copyFile(src, dst string) error {
	in, err := os.Open(src)
	if err != nil {
		return err
	}
	defer in.Close()

	out, err := os.Create(dst)
	if err != nil {
		return err
	}
	defer out.Close()

	if _, err := io.Copy(out, in); err != nil {
		return err
	}

	return out.Close()
}
