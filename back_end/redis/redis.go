package redis

import (
	"IntelligentHome/config"
	"log"

	"github.com/go-redis/redis"
)

var DB *redis.Client

// 初始化连接
func Init() {

	DB = redis.NewClient(&redis.Options{
		Addr:     config.ConfigContext.RedisConfig.Host + ":" + config.ConfigContext.RedisConfig.Port,
		Password: config.ConfigContext.RedisConfig.Password, // no password set
		DB:       config.ConfigContext.RedisConfig.DB,       // use default DB
	})
	var err error
	_, err = DB.Ping().Result()
	if err != nil {
		log.Println("Redis connect failed")
	} else {
		log.Println("Redis connect success")
	}

}

func ZAdd(key string, score float64, member interface{}) (err error) {
	err = DB.ZAdd(key, redis.Z{Score: score, Member: member}).Err()
	return err
}

func ZRangeByScore(key string, count int64) (res []string, err error) {
	res, err = DB.ZRevRangeByScore(key, redis.ZRangeBy{Min: "-inf", Max: "+inf", Count: count}).Result()
	return res, err
}
