package config

import (
	"encoding/json"
	"log"
	"os"
)

type RedisConfig struct {
	Host     string `json:"host"`
	Port     string `json:"port"`
	Username string `json:"username"`
	Password string `json:"password"`
	DB       int    `json:"db"`
	KeyName  string `json:"key_name"` // 存储数据的key名称
	MaxData  int    `json:"max_data"` // 最大数据量，单位：秒
}

type ApiConfig struct {
	Host string `json:"host"`
	Port string `json:"port"`
	Mode string `json:"mode"`
}

type ChuanKouConfig struct {
	Name string `json:"name"`
	Baud int    `json:"baud"`
}

type Config struct {
	ApiConfig      ApiConfig      `json:"api_config"`
	RedisConfig    RedisConfig    `json:"redis_config"`
	ChuanKouConfig ChuanKouConfig `json:"chuan_kou_config"`
}

var ConfigContext Config

var ConfigFileName = "./config.json"

var DefaultConfig = Config{
	ApiConfig: ApiConfig{
		Host: "0.0.0.0",
		Port: "1314",
		Mode: "release",
	},
	RedisConfig: RedisConfig{
		Host:     "127.0.0.1",
		Port:     "6379",
		Username: "",
		Password: "",
		DB:       0,
		KeyName:  "ihome",
		MaxData:  3600,
	},
	ChuanKouConfig: ChuanKouConfig{
		Name: "COM2",
		Baud: 9600,
	},
}

func Init() {
	log.Println("读取配置文件")
	ConfigFile, err := os.ReadFile(ConfigFileName)
	if err != nil {
		log.Println("配置文件不存在，使用默认配置")
		ConfigContext = DefaultConfig
		data, _ := json.Marshal(DefaultConfig)
		os.WriteFile(ConfigFileName, data, 0644)
		return
	}
	err = json.Unmarshal(ConfigFile, &ConfigContext)
	if err != nil {
		log.Panicln(err)
		return
	}
	log.Println("读取配置文件成功")
}
