package logger

import (
	"os"
	"path/filepath"

	"github.com/MaaXYZ/maa-boom-aid/internal/config"
	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
	"gopkg.in/natefinch/lumberjack.v2"
)

func New(conf *config.Config) *zap.Logger {
	exePath, err := os.Executable()
	if err != nil {
		return nil
	}
	exeDir := filepath.Dir(exePath)
	logPath := filepath.Join(exeDir, "debug", "log.jsonl")
	hook := lumberjack.Logger{
		Filename:   logPath,
		MaxSize:    conf.Log.MaxSize,
		MaxBackups: conf.Log.MaxBackups,
		MaxAge:     conf.Log.MaxAge,
		Compress:   conf.Log.Compress,
	}

	encoder := getJsonEncoder()
	core := zapcore.NewCore(
		encoder,
		zapcore.AddSync(&hook),
		getLevel(conf.Log.Level),
	)
	return zap.New(core, zap.AddCaller(), zap.AddStacktrace(zap.ErrorLevel))
}

func getLevel(level string) zapcore.Level {
	switch level {
	case "debug":
		return zap.DebugLevel
	case "info":
		return zap.InfoLevel
	case "warn":
		return zap.WarnLevel
	case "error":
		return zap.ErrorLevel
	case "fatal":
		return zap.FatalLevel
	default:
		return zap.InfoLevel
	}
}

func getLumberjackLogger(conf *config.Config) (lumberjack.Logger, error) {
	exePath, err := os.Executable()
	if err != nil {
		return lumberjack.Logger{}, err
	}
	exeDir := filepath.Dir(exePath)
	logPath := filepath.Join(exeDir, "debug", "log.jsonl")
	return lumberjack.Logger{
		Filename:   logPath,
		MaxSize:    conf.Log.MaxSize,
		MaxBackups: conf.Log.MaxBackups,
		MaxAge:     conf.Log.MaxAge,
		Compress:   conf.Log.Compress,
	}, nil
}

func getJsonEncoder() zapcore.Encoder {
	conf := zap.NewProductionEncoderConfig()
	conf.TimeKey = "time"
	conf.EncodeTime = zapcore.ISO8601TimeEncoder
	return zapcore.NewJSONEncoder(conf)
}
