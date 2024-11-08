package api

import (
	"IntelligentHome/homecontrol"
	"net/http"

	"github.com/gin-gonic/gin"
)

func RunApi() {
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
				LightsOn:    []int{0, 50, 101},
				CurtainsON:  []int{0, 50, 100},
				SocketsOn:   []bool{true, false, true},
			},
			{
				Name:        "room2",
				Temperature: 25,
				Humidity:    60,
				LightsOn:    []int{0, 50, 101},
				CurtainsON:  []int{0, 50, 100},
				SocketsOn:   []bool{true, false, true},
			},
			{
				Name:        "客厅",
				Temperature: 24,
				Humidity:    60,
				LightsOn:    []int{0, 50, 101},
				CurtainsON:  []int{0, 50, 100},
				SocketsOn:   []bool{true, false, true},
			},
		}
		ctx.JSONP(200, gin.H{
			"stat": stat,
		})
	})

	r.POST("/stat/control", func(c *gin.Context) {
		json := homecontrol.Room{}
		c.BindJSON(&json)
		c.JSON(http.StatusOK, gin.H{
			"stat": json,
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
