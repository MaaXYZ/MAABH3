package config

import (
	"log"
	"os"
	"path/filepath"
	"strings"

	"github.com/pelletier/go-toml/v2"
	"github.com/spf13/viper"
)

type Config struct {
	AdbPath string          `mapstructure:"adb_path" toml:"adb_path"`
	Log     *LogConfig      `mapstructure:"log" toml:"log"`
	Taskers []*TaskerConfig `mapstructure:"taskers" toml:"taskers"`
}

type LogConfig struct {
	Level string `mapstructure:"level" toml:"level"`
}

type TaskerConfig struct {
	ID          string            `mapstructure:"id" toml:"id"`
	Name        string            `mapstructure:"name" toml:"name"`
	CtrlType    string            `mapstructure:"ctrl_type" toml:"ctrl_type"`
	Bundles     []string          `mapstructure:"bundles" toml:"bundles"`
	Win32Window Win32WindowConfig `mapstructure:"win32_window" toml:"win32_window"`
	AdbDevice   AdbDeviceConfig   `mapstructure:"adb_device" toml:"adb_device"`
	Tasks       []Task            `mapstructure:"tasks" toml:"tasks"`
}

type Win32WindowConfig struct {
	Screencap string `mapstructure:"screencap" toml:"screencap"`
	Input     string `mapstructure:"intpu" toml:"intpu"`
}

type AdbDeviceConfig struct {
	SerialNumber string                 `mapstructure:"serial_number" toml:"serial_number"`
	Screencap    string                 `mapstructure:"screencap" toml:"screencap"`
	Input        string                 `mapstructure:"input" toml:"input"`
	Config       map[string]interface{} `mapstructure:"config" toml:"config"`
}

type Task struct {
	Entry string                 `mapstructure:"entry" toml:"entry"`
	Param map[string]interface{} `mapstructure:"param" toml:"param"`
}

func New() *Config {
	v := viper.New()

	v.SetDefault("server.port", 8000)
	v.SetDefault("device.adb_config", map[string]interface{}{})

	v.SetConfigName("config")
	v.SetConfigType("toml")

	exePath, err := os.Executable()
	if err != nil {
		log.Fatalf("Failed to get executable path, %v", err)
	}
	exeDir := filepath.Dir(exePath)
	configDir := filepath.Join(exeDir, "config")
	v.AddConfigPath(configDir)

	if err := v.ReadInConfig(); err != nil {
		log.Fatalf("Failed to read config file, %v", err)
	}

	var config Config
	if err := v.Unmarshal(&config); err != nil {
		log.Fatalf("Failed to unmarshal config file, %v", err)
	}

	for _, tasker := range config.Taskers {
		for i, bundle := range tasker.Bundles {
			tasker.Bundles[i] = strings.Replace(bundle, "{PROJECT_DIR}", exeDir, -1)
		}
	}

	return &config
}

func (c *Config) Save() error {
	exePath, err := os.Executable()
	if err != nil {
		return err
	}
	exeDir := filepath.Dir(exePath)
	configDir := filepath.Join(exeDir, "config")
	configFile := filepath.Join(configDir, "config.toml")

	file, err := os.Create(configFile)
	if err != nil {
		return err
	}
	defer file.Close()

	encode := toml.NewEncoder(file)

	return encode.Encode(c)
}
