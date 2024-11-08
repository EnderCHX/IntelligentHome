package redis

import (
	"github.com/go-redis/redis"
)

var DB *redis.Client

// 初始化连接
func InitClient() (err error) {
	DB = redis.NewClient(&redis.Options{
		Addr:     "localhost:6379",
		Password: "", // no password set
		DB:       0,  // use default DB
	})

	_, err = DB.Ping().Result()
	if err != nil {
		return err
	}
	return nil
}
