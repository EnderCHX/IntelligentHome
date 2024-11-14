package setup

import (
	"IntelligentHome/api"
	"IntelligentHome/config"
	"IntelligentHome/redis"
	"IntelligentHome/serial"
)

func Init() {
	config.Init()
	redis.Init()
	serial.Init()
	api.RunApi()
}
