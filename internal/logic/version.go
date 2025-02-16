package logic

import (
	"github.com/MaaXYZ/maa-boom-aid/internal/config"
	"github.com/MaaXYZ/maa-framework-go"
)

type Version struct{}

func NewVersion() *Version {
	return &Version{}
}

func (l *Version) GetMaaFrameworkVersion() string {
	return maa.Version()
}

func (l *Version) GetElfAidMagicVersion() string {
	return config.Version
}

func (l *Version) GetGoVersion() string {
	return config.GoVersion
}

func (l *Version) GetBuildAt() string {
	return config.BuildAt
}
