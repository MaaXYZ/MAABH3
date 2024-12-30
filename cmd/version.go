package cmd

import (
	"fmt"

	"github.com/MaaXYZ/maa-boom-aid/internal/config"
	"github.com/spf13/cobra"
)

var versionCmd = &cobra.Command{
	Use:   "version",
	Short: "Print the version of maa-boom-aid",
	Run: func(cmd *cobra.Command, args []string) {
		version()
	},
}

func version() {
	fmt.Println("Build At:", config.BuildAt)
	fmt.Println("Go Version:", config.GoVersion)
	fmt.Println("Version:", config.Version)
}

func init() {
	rootCmd.AddCommand(versionCmd)
}
