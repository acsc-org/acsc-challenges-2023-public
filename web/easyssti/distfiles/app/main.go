package main

import (
	"bytes"
	"fmt"
	"html/template"
	"net/http"
	"os"

	"github.com/labstack/echo/v4"
	"github.com/labstack/echo/v4/middleware"
)

func templateMiddleware(next echo.HandlerFunc) echo.HandlerFunc {
	return func(c echo.Context) error {
		file, err := os.Open("./template.html")
		if err != nil {
			return err
		}
		stat, err := file.Stat()
		if err != nil {
			return err
		}
		buf := make([]byte, stat.Size())
		_, err = file.Read(buf)
		if err != nil {
			return err
		}

		userTemplate := c.Request().Header.Get("Template")

		if userTemplate != "" {
			buf = []byte(userTemplate)
		}

		c.Set("template", buf)
		return next(c)
	}
}

func handleIndex(c echo.Context) error {
	tmpl, ok := c.Get("template").([]byte)

	if !ok {
		return fmt.Errorf("failed to get template")
	}

	tmplStr := string(tmpl)
	t, err := template.New("page").Parse(tmplStr)
	if err != nil {
		return c.String(http.StatusInternalServerError, err.Error())
	}

	buf := new(bytes.Buffer)

	if err := t.Execute(buf, c); err != nil {
		return c.String(http.StatusInternalServerError, err.Error())
	}

	return c.HTML(http.StatusOK, buf.String())
}

func main() {
	e := echo.New()

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())

	e.GET("/", handleIndex, templateMiddleware)

	e.Logger.Fatal(e.Start(":3001"))
}
