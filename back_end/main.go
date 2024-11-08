package main

import (
	"IntelligentHome/api"
	"IntelligentHome/redis"
)

func main() {
	redis.InitClient()
	api.RunApi()
}
