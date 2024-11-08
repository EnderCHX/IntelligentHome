package main

import (
	"net/http"

	"demo/homecontrol"

	"github.com/gin-gonic/gin"
)

func main() {
	r := gin.Default()
	r.Use(Cors())

	const (
		StaticPath    = "./static"
		StaticWebPath = "static"
		TemplatesPath = "./templates/*"
	)
	r.LoadHTMLGlob(TemplatesPath)
	r.Static(StaticWebPath, StaticPath)

	r.StaticFile("/favicon.ico", "./static/favicon.ico")

	r.GET("/", func(c *gin.Context) {
		c.HTML(http.StatusOK, "index.html", gin.H{
			"data":   "ciallo world",
			"static": StaticWebPath,
		})
	})

	r.GET("/ping", func(c *gin.Context) {
		c.JSON(200, gin.H{
			"message": "pong",
		})
	})

	r.GET("/stat", func(ctx *gin.Context) {
		stat := []homecontrol.Room{
			{
				Name:        "room1",
				Temperature: 26,
				Humidity:    60,
				LightsOn:    []bool{true, false, true},
				CurtainsON:  []bool{true, false, true},
			},
			{
				Name:        "room2",
				Temperature: 26,
				Humidity:    60,
				LightsOn:    []bool{true, false, true},
				CurtainsON:  []bool{true, false, true},
			},
		}
		ctx.JSON(200, gin.H{
			"stat": stat,
		})
	})
	r.Run()
}

func Cors() gin.HandlerFunc {
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
