package main

import (
	"IntelligentHome/api"
	"IntelligentHome/compim"
	"IntelligentHome/redis"
	"log"
)

func main() {
	redis.InitClient()
	go ReadAd()

	api.RunApi()
}

func ReadAd() {
	for {
		data := compim.Read("COM2", 9600, "#")
		log.Println(data)
	}
}
