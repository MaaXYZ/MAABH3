package config

import (
	"log"
	"os"
	"path/filepath"

	"github.com/MaaXYZ/maa-framework-go"
	"github.com/pelletier/go-toml/v2"
	"github.com/spf13/viper"
)

type Config struct {
	Log     *LogConfig      `mapstructure:"log" toml:"log"`
	Taskers []*TaskerConfig `mapstructure:"taskers" toml:"taskers"`
}

type LogConfig struct {
	Level      string `mapstructure:"level" toml:"level"`
	MaxSize    int    `mapstructure:"max_size" toml:"max_size"`
	MaxBackups int    `mapstructure:"max_backups" toml:"max_backups"`
	MaxAge     int    `mapstructure:"max_age" toml:"max_age"`
	Compress   bool   `mapstructure:"compress" toml:"compress"`
}

type TaskerConfig struct {
	ID          string            `mapstructure:"id" toml:"id"`
	Name        string            `mapstructure:"name" toml:"name"`
	CtrlType    string            `mapstructure:"ctrl_type" toml:"ctrl_type"`
	Win32Window Win32WindowConfig `mapstructure:"win32_window" toml:"win32_window"`
	AdbDevice   AdbDeviceConfig   `mapstructure:"adb_device" toml:"adb_device"`
	Tasks       []Task            `mapstructure:"tasks" toml:"tasks"`
}

type Win32WindowConfig struct {
	Screencap string `mapstructure:"screencap" toml:"screencap"`
	Input     string `mapstructure:"intpu" toml:"intpu"`
}

func (w *Win32WindowConfig) GetScreencapMethod() maa.Win32ScreencapMethod {
	return strToWin32CtrlScreencapMethod(w.Screencap)
}

func (w *Win32WindowConfig) GetInputMethod() maa.Win32InputMethod {
	return strToWin32CtrlInputMethod(w.Input)
}

type AdbDeviceConfig struct {
	SerialNumber string                 `mapstructure:"serial_number" toml:"serial_number"`
	Screencap    string                 `mapstructure:"screencap" toml:"screencap"`
	Input        string                 `mapstructure:"input" toml:"input"`
	Config       map[string]interface{} `mapstructure:"config" toml:"config"`
}

func (a *AdbDeviceConfig) GetScreencapMethod() maa.AdbScreencapMethod {
	return strToAdbCtrlScreencapMethod(a.Screencap)
}

func (a *AdbDeviceConfig) GetInputMethod() maa.AdbInputMethod {
	return strToAdbCtrlInputMethod(a.Input)
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

func strToAdbCtrlScreencapMethod(method string) maa.AdbScreencapMethod {
	switch method {
	case "Default":
		return maa.AdbScreencapMethodDefault
	case "EncodeToFileAndPull":
		return maa.AdbScreencapMethodEncodeToFileAndPull
	case "Encode":
		return maa.AdbScreencapMethodEncode
	case "RawWithGzip":
		return maa.AdbScreencapMethodRawWithGzip
	case "RawByNetcat":
		return maa.AdbScreencapMethodRawByNetcat
	case "MinicapDirect":
		return maa.AdbScreencapMethodMinicapDirect
	case "MinicapStream":
		return maa.AdbScreencapMethodMinicapStream
	case "EmulatorExtras":
		return maa.AdbScreencapMethodEmulatorExtras
	default:
		return maa.AdbScreencapMethodNone
	}
}

func strToAdbCtrlInputMethod(method string) maa.AdbInputMethod {
	switch method {
	case "Default":
		return maa.AdbInputMethodDefault
	case "AdbShell":
		return maa.AdbInputMethodAdbShell
	case "MinitouchAndAdbKey":
		return maa.AdbInputMethodMinitouchAndAdbKey
	case "Maatouch":
		return maa.AdbInputMethodMaatouch
	case "EmulatorExtras":
		return maa.AdbInputMethodEmulatorExtras
	default:
		return maa.AdbInputMethodNone
	}
}

func strToWin32CtrlScreencapMethod(method string) maa.Win32ScreencapMethod {
	switch method {
	case "GDI":
		return maa.Win32ScreencapMethodGDI
	case "FramePool":
		return maa.Win32ScreencapMethodFramePool
	case "DXGIDesktopDup":
		return maa.Win32ScreencapMethodDXGIDesktopDup
	default:
		return maa.Win32ScreencapMethodNone
	}
}

func strToWin32CtrlInputMethod(method string) maa.Win32InputMethod {
	switch method {
	case "Seize":
		return maa.Win32InputMethodSeize
	case "SendMessage":
		return maa.Win32InputMethodSendMessage
	default:
		return maa.Win32InputMethodNone
	}
}
