package redis

import (
	"IntelligentHome/config"
	"errors"
	"log"
	"time"

	"github.com/go-redis/redis"
)

var DB *redis.Client
var KeyName string

// 初始化连接
func Init() {

	DB = redis.NewClient(&redis.Options{
		Addr:     config.ConfigContext.RedisConfig.Host + ":" + config.ConfigContext.RedisConfig.Port,
		Password: config.ConfigContext.RedisConfig.Password, // no password set
		DB:       config.ConfigContext.RedisConfig.DB,       // use default DB
	})

	KeyName = config.ConfigContext.RedisConfig.KeyName

	var err error
	_, err = DB.Ping().Result()
	if err != nil {
		log.Println("Redis connect failed")
	} else {
		log.Println("Redis connect success")
	}

	go KeepMaxData()
}

func KeepMaxData() {
	MaxData := config.ConfigContext.RedisConfig.MaxData
	WaitTime := time.Minute * 5
	for {
		currentSize, err := DB.ZCard(KeyName).Result()
		if err != nil {
			log.Println(err)
			return
		}
		overflow := currentSize - int64(MaxData)
		if overflow > 0 {
			log.Printf("Redis data size is too large, delete the oldest %d data", overflow)
			for i := 0; i < int(overflow); i++ {
				DB.ZRemRangeByRank(KeyName, 0, 0)
			}
		}

		time.Sleep(WaitTime)
	}

}

func ZAdd(key string, score float64, member interface{}) (err error) {
	err = DB.ZAdd(key, redis.Z{Score: score, Member: member}).Err()
	return err
}

func ZRevRangeByScore(count int64, key ...string) (res []string, err error) {
	if len(key) == 0 {
		res, err = DB.ZRevRangeByScore(KeyName, redis.ZRangeBy{Min: "-inf", Max: "+inf", Count: count}).Result()
		return res, err
	}
	if len(key) > 1 {
		err = errors.New("ZRevRangeByScore only support zero or one key")
		return nil, err
	}
	res, err = DB.ZRevRangeByScore(key[0], redis.ZRangeBy{Min: "-inf", Max: "+inf", Count: count}).Result()
	return res, err
}

func ZRevRangeByScoreWithScores(count int64, key ...string) (data []redis.Z, err error) {
	if len(key) == 0 {
		data, err = DB.ZRevRangeByScoreWithScores(KeyName, redis.ZRangeBy{Min: "-inf", Max: "+inf", Count: count}).Result()
		return data, err
	}
	if len(key) > 1 {
		err = errors.New("ZRevRangeByScoreWithScores only support zero or one key")
		return nil, err
	}
	data, err = DB.ZRevRangeByScoreWithScores(key[0], redis.ZRangeBy{Min: "-inf", Max: "+inf", Count: count}).Result()
	return data, err
}
