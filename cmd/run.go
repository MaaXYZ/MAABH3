package cmd

import (
	"os"
	"path/filepath"

	"github.com/MaaXYZ/maa-boom-aid/internal/config"
	"github.com/MaaXYZ/maa-boom-aid/internal/logger"
	"github.com/MaaXYZ/maa-framework-go/v2"
	"github.com/bytedance/sonic"
	"github.com/rs/zerolog/log"
	"github.com/spf13/cobra"
)

var runCmd = &cobra.Command{
	Use:   "run",
	Short: "run maa-boom-aid",
	Run: func(cmd *cobra.Command, args []string) {
		run()
	},
}

func run() {
	conf := config.New()
	logger.Init(conf)

	log.Info().
		Msg("start")

	if len(conf.Taskers) == 0 {
		log.Fatal().
			Msg("no taskers found")
	}

	tasker := maa.NewTasker(nil)
	if tasker == nil {
		log.Fatal().
			Msg("failed to create tasker")
	}

	res := maa.NewResource(nil)
	if res == nil {
		log.Fatal().
			Msg("failed to create resource")
	}
	defer res.Destroy()

	bundles := conf.Taskers[0].Bundles
	for i, bundle := range bundles {
		if res.PostBundle(bundle).Wait().Success() {
			log.Info().
				Int("index", i).
				Str("path", bundle).
				Msg("bundle post success")
		} else {
			log.Fatal().
				Int("index", i).
				Str("path", bundle).
				Msg("bundle post failed")
		}
	}

	if tasker.BindResource(res) {
		log.Info().
			Msg("resource bind success")
	} else {
		log.Fatal().
			Msg("resource bind failed")
	}

	device := conf.Taskers[0].AdbDevice

	var adbConfigStr string
	adbConfigData, err := sonic.Marshal(device.Config)
	if err != nil {
		log.Fatal().
			Err(err).
			Msg("failed to marshal adb config")
	}
	if device.Config == nil {
		adbConfigStr = "{}"
	} else {
		adbConfigStr = string(adbConfigData)
	}

	exePath, err := os.Executable()
	if err != nil {
		log.Fatal().
			Err(err).
			Msg("failed to get executable path")
	}
	exeDir := filepath.Dir(exePath)
	maaAgentBinaryDir := filepath.Join(exeDir, "MaaAgentBinary")

	ctrl := maa.NewAdbController(
		conf.AdbPath,
		device.SerialNumber,
		device.GetScreencapMethod(),
		device.GetInputMethod(),
		adbConfigStr,
		maaAgentBinaryDir,
		nil,
	)
	if ctrl == nil {
		log.Fatal().
			Msg("failed to create adb controller")
	}
	defer ctrl.Destroy()

	if ctrl.PostConnect().Wait().Success() {
		log.Info().
			Msg("adb connect success")
	} else {
		log.Fatal().
			Msg("adb connect failed")
	}

	if tasker.BindController(ctrl) {
		log.Info().
			Msg("controller bind success")
	} else {
		log.Fatal().
			Msg("controller bind failed")
	}

	taskList := conf.Taskers[0].Tasks
	if len(taskList) == 0 {
		log.Warn().
			Msg("no tasks found")
		return
	}

	for _, task := range taskList {
		if task.Entry == "" {
			log.Warn().
				Msg("task entry is empty")
			continue
		}

		log.Info().
			Str("task", task.Entry).
			Msg("start task")
		if tasker.PostTask(task.Entry, task.Param).Wait().Success() {
			log.Info().
				Str("task", task.Entry).
				Msg("task post success")
		} else {
			log.Error().
				Str("task", task.Entry).
				Msg("task post failed")
		}
	}

	log.Info().
		Msg("done")

}

func init() {
	rootCmd.AddCommand(runCmd)
}
