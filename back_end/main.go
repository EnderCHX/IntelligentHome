package main

import (
	"IntelligentHome/api"
	"IntelligentHome/compim"
	"IntelligentHome/config"
	"IntelligentHome/redis"
	"regexp"
	"time"
)

func main() {
	config.Init()
	compim.Init()
	redis.Init()

	go ReadAd()
	api.RunApi()
	// res, _ := redis.ZRangeByScore("ihome", 2)
	// for _, v := range res {
	// 	log.Println(v)
	// }
}

func ReadAd() { //读取数据存入数据库
	data := ""
	re := regexp.MustCompile(`#\{(.*?)\}#`)
	for {
		// time.Sleep(1 * time.Second)
		data += compim.Read()
		matches := re.FindStringSubmatch(data)
		if len(matches) > 1 {
			// log.Println("Extracted text:", matches[1]) // matches[1]是第一个括号中的内容
			redis.ZAdd("ihome", float64(time.Now().Unix()), matches[1])
			data = ""
		}
		// } else {
		// 	// log.Println("No match found")
		// }
	}
}
