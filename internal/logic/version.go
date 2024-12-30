package logic

import (
	"github.com/MaaXYZ/maa-boom-aid/internal/config"
	"github.com/MaaXYZ/maa-framework-go"
)

type VersionLogic struct{}

func NewVersionLogic() *VersionLogic {
	return &VersionLogic{}
}

func (l *VersionLogic) GetMaaFrameworkVersion() string {
	return maa.Version()
}

func (l *VersionLogic) GetElfAidMagicVersion() string {
	return config.Version
}

func (l *VersionLogic) GetGoVersion() string {
	return config.GoVersion
}

func (l *VersionLogic) GetBuildAt() string {
	return config.BuildAt
}
