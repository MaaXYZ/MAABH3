package cmd

import (
	"fmt"

	"github.com/MaaXYZ/maa-boom-aid/internal/logic"
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
	versionLogic := logic.NewVersionLogic()
	fmt.Println("Build At:", versionLogic.GetBuildAt())
	fmt.Println("Go Version:", versionLogic.GetGoVersion())
	fmt.Println("Version:", versionLogic.GetElfAidMagicVersion())
	fmt.Println("Maa Framework Version:", versionLogic.GetMaaFrameworkVersion())
}

func init() {
	rootCmd.AddCommand(versionCmd)
}
