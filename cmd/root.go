package cmd

import (
	_ "github.com/MaaXYZ/maa-boom-aid/internal/logger"
	"github.com/rs/zerolog/log"
	"github.com/spf13/cobra"
)

var rootCmd = &cobra.Command{
	Use:   "mba",
	Short: "A one-click tool for the daily tasks of Honkai Impact.",
}

func Execute() {
	if err := rootCmd.Execute(); err != nil {
		log.Fatal().
			Err(err).
			Msg("failed to execute command")
	}
}
