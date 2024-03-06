package main

import (
	"errors"
	"html/template"
	"net"
	"net/http"
	"os"
	"path/filepath"
	"regexp"
	"strings"

	"github.com/dpapathanasiou/go-recaptcha"
	"github.com/google/uuid"
)

type NoteParam struct {
	Id               string
	URL              string
	Title            string
	Body             string
	RecaptchaSiteKey string
}

const (
	PublicDir    = "./public"
	NoteBaseDir  = "./notes"
	NoteTemplate = "./templates/note.html"
)

func WriteNote(file *os.File, body NoteParam) {
	body.RecaptchaSiteKey = os.Getenv("RECAPTCHA_SITEKEY")

	tmpl, err := template.ParseFiles(NoteTemplate)
	if err != nil {
		panic(err)
	}

	err = tmpl.Execute(file, body)
	if err != nil {
		panic(err)
	}
}

func GenerateNoteId(title string) string {
	prefix := uuid.New().String()
	id := prefix + "-" + strings.ReplaceAll(title, " ", "-")
	return id
}

func GetNotePath(noteId string) (string, string) {
	notePublicPath := filepath.Join(NoteBaseDir, noteId)
	noteFilePath := filepath.Join(PublicDir, notePublicPath)

	return noteFilePath, notePublicPath
}

func ValidateNoteBody(body string) error {
	if len(body) > 1024 {
		return errors.New("note is too long")
	}
	return nil
}
func ValidateNoteTitle(title string) error {
	if len(title) > 20 {
		return errors.New("title is too long")
	}

	validTitleRegExp := regexp.MustCompile("^[a-zA-Z0-9 ]+$")
	if !validTitleRegExp.MatchString(title) {
		return errors.New("title is invalid")
	}
	return nil
}
func ValidateNoteId(id string) error {
	if len(id) > 60 {
		return errors.New("id is too long")
	}

	validTitleRegExp := regexp.MustCompile("^[a-zA-Z0-9-]+$")
	if !validTitleRegExp.MatchString(id) {
		return errors.New("id is invalid")
	}
	return nil
}

func main() {
	recaptcha.Init(os.Getenv("RECAPTCHA_SECRETKEY"))

	http.HandleFunc("/new-note", func(w http.ResponseWriter, r *http.Request) {
		body := r.FormValue("body")
		title := r.FormValue("title")

		// validate body
		err := ValidateNoteBody(body)
		if err != nil {
			http.Error(w, "invalid body", http.StatusInternalServerError)
			return
		}

		// validate title
		err = ValidateNoteTitle(title)
		if err != nil {
			http.Error(w, "invalid title", http.StatusInternalServerError)
			return
		}

		noteId := GenerateNoteId(title)
		noteFilePath, noteURL := GetNotePath(noteId)

		f, err := os.Create(noteFilePath)
		if err != nil {
			http.Error(w, "failed note creation", http.StatusInternalServerError)
			return
		}

		WriteNote(f, NoteParam{
			Id:    noteId,
			URL:   noteURL,
			Title: title,
			Body:  body,
		})

		http.Redirect(w, r, noteURL, http.StatusFound)
	})

	http.HandleFunc("/update-note", func(w http.ResponseWriter, r *http.Request) {
		noteId := r.FormValue("noteId")
		title := r.FormValue("title")
		body := r.FormValue("body")

		// validate noteId
		err := ValidateNoteId(noteId)
		if err != nil {
			http.Error(w, "invalid id", http.StatusInternalServerError)
			return
		}

		// validate title
		err = ValidateNoteTitle(title)
		if err != nil {
			http.Error(w, "invalid title", http.StatusInternalServerError)
			return
		}

		// validate body
		err = ValidateNoteBody(body)
		if err != nil {
			http.Error(w, "invalid body", http.StatusInternalServerError)
			return
		}

		noteFilePath, noteURL := GetNotePath(noteId)

		f, err := os.OpenFile(noteFilePath, os.O_WRONLY|os.O_TRUNC, 0644)
		if err != nil {
			http.Error(w, "invalid note", http.StatusInternalServerError)
			return
		}

		WriteNote(f, NoteParam{
			Id:    noteId,
			URL:   noteURL,
			Title: title,
			Body:  body,
		})

		http.Redirect(w, r, noteURL, http.StatusFound)
	})

	http.HandleFunc("/report", func(w http.ResponseWriter, r *http.Request) {
		path := r.FormValue("path")
		recaptchaResponse := r.FormValue("g-recaptcha-response")

		// recaptcha
		result, err := recaptcha.Confirm(r.RemoteAddr, recaptchaResponse)
		if err != nil || !result {
			http.Error(w, "captcha failed", http.StatusInternalServerError)
			return
		}

		conn, err := net.Dial("tcp", "bot:3000")
		if err != nil {
			http.Error(w, "report failed", http.StatusInternalServerError)
			return
		}
		defer conn.Close()

		conn.Write([]byte(path))

		w.Write([]byte("reported"))
	})

	http.Handle("/", http.FileServer(http.Dir(PublicDir)))

	http.ListenAndServe(":3000", nil)
}
