package logger

import (
	"os"
	"path/filepath"
	"time"

	"github.com/MaaXYZ/maa-boom-aid/internal/config"
	"github.com/rs/zerolog"
	"github.com/rs/zerolog/log"
	"gopkg.in/natefinch/lumberjack.v2"
)

func Init(conf *config.Config) {
	exePath, err := os.Executable()
	if err != nil {
		log.Fatal().
			Err(err).
			Msg("failed to get executable path")
	}

	level, err := zerolog.ParseLevel(conf.Log.Level)
	if err != nil {
		log.Fatal().
			Err(err).
			Msg("failed to parse log level")
	}

	exeDir := filepath.Dir(exePath)
	logPath := filepath.Join(exeDir, "debug", "mba.log")

	writer := zerolog.MultiLevelWriter(
		&lumberjack.Logger{
			Filename:   logPath,
			MaxSize:    16, // megabytes
			MaxBackups: 3,  // files
			MaxAge:     90, // days
			Compress:   true,
		},
		zerolog.ConsoleWriter{
			Out:        os.Stdout,
			TimeFormat: time.RFC3339Nano,
		},
	)

	log.Logger = zerolog.New(writer).
		With().
		Timestamp().
		Caller().
		Logger().
		Level(level)
}

func init() {
	zerolog.TimeFieldFormat = time.RFC3339Nano
	zerolog.DurationFieldUnit = time.Nanosecond
}
