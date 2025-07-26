package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"strings"
)

// Regex for detecting typedef blocks and function signatures
var (
	typedefRegex = regexp.MustCompile(`^\s*typedef\b`)
	funcSigRegex = regexp.MustCompile(`^\s*[a-zA-Z_][a-zA-Z0-9_\s\*]*\s+[a-zA-Z_][a-zA-Z0-9_]*\s*\([^;{]*\)\s*\{`)
)

func extractHeaders(inputFile, headerFile string) error {
	file, err := os.Open(inputFile)
	if err != nil {
		return fmt.Errorf("failed to open %s: %v", inputFile, err)
	}
	defer file.Close()

	headers := []string{}
	scanner := bufio.NewScanner(file)

	// State for typedef multi-line reading
	inTypedef := false
	typedefBuffer := strings.Builder{}

	for scanner.Scan() {
		line := scanner.Text()

		// === Handle typedef blocks ===
		if inTypedef {
			typedefBuffer.WriteString(line + "\n")
			if strings.Contains(line, ";") {
				// End of typedef
				headers = append(headers, strings.TrimSpace(typedefBuffer.String()))
				typedefBuffer.Reset()
				inTypedef = false
			}
			continue
		}

		if typedefRegex.MatchString(line) {
			inTypedef = true
			typedefBuffer.WriteString(line + "\n")
			if strings.Contains(line, ";") {
				// Single-line typedef
				headers = append(headers, strings.TrimSpace(typedefBuffer.String()))
				typedefBuffer.Reset()
				inTypedef = false
			}
			continue
		}

		// === Handle function signatures ===
		if funcSigRegex.MatchString(line) {
			// Clean up the signature by removing the function body start
			signature := strings.TrimSpace(funcSigRegex.ReplaceAllStringFunc(line, func(match string) string {
				return strings.TrimSuffix(strings.TrimSpace(match), "{")
			}))
			// Ensure it ends with ;
			if !strings.HasSuffix(signature, ";") {
				signature += ";"
			}
			headers = append(headers, signature)
		}
	}

	// Append to headers.h
	hFile, err := os.OpenFile(headerFile, os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		return fmt.Errorf("failed to open %s: %v", headerFile, err)
	}
	defer hFile.Close()

	writer := bufio.NewWriter(hFile)
	for _, hdr := range headers {
		writer.WriteString(hdr + "\n")
	}
	writer.Flush()

	return nil
}

func main() {
	if len(os.Args) != 3 {
		fmt.Println("Usage: go run main.go <source.c> <headers.h>")
		return
	}

	source := os.Args[1]
	header := os.Args[2]

	if err := extractHeaders(source, header); err != nil {
		fmt.Printf("Error: %v\n", err)
	} else {
		fmt.Printf("Headers appended to %s\n", header)
	}
}
