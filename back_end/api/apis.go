package api

import (
	"IntelligentHome/homecontrol"
	"IntelligentHome/redis"
	"net/http"
	"strconv"

	"github.com/gin-gonic/gin"
)

func Cors() gin.HandlerFunc { //跨域中间件
	return func(c *gin.Context) {
		method := c.Request.Method
		origin := c.Request.Header.Get("Origin")
		if origin != "" {
			c.Header("Access-Control-Allow-Origin", "*") // 可将将 * 替换为指定的域名
			c.Header("Access-Control-Allow-Methods", "POST, GET, OPTIONS, PUT, DELETE, UPDATE")
			c.Header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, Authorization")
			c.Header("Access-Control-Expose-Headers", "Content-Length, Access-Control-Allow-Origin, Access-Control-Allow-Headers, Cache-Control, Content-Language, Content-Type")
			c.Header("Access-Control-Allow-Credentials", "true")
		}
		if method == "OPTIONS" {
			c.AbortWithStatus(http.StatusNoContent)
		}
		c.Next()
	}
}

func GetStat(c *gin.Context) {
	count := c.DefaultQuery("count", "1")
	count, err := strconv.Atoi(count)
	keyname := "ihome"
	res, err := redis.ZRangeByScore(keyname, count)
	if err != nil {
		c.JSON(http.StatusOK, gin.H{
			"error": err.Error(),
		})
		return
	}
	c.JSONP(200, gin.H{
		"stats": res,
	})
}

func StatControl(c *gin.Context) {
	json := homecontrol.Room{}
	c.BindJSON(&json)
	c.JSON(http.StatusOK, gin.H{
		"stat": json,
	})
}
