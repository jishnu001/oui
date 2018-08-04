package main

import (
    "fmt"
    "io/ioutil"
    "log"
    "os"
    "regexp"
    "strings"
)

//var ouiMap map[string]string

func main() {

    ouiMap := make(map[string]string)

    file, err := ioutil.ReadFile("../oui.txt")
    if err != nil {
        log.Fatal(err)
    }

    //fmt.Println(file) // print the content as 'bytes'

    str := string(file) // convert content to a 'string'

    list := strings.SplitAfter(str, "\n")

    for i := 0; i < len(list); i++ {

        if strings.Contains(list[i], "(hex)") {

            a := strings.Index(list[i], "(")
            mac := list[i][:a]
            mac = strings.TrimSpace(mac)
            //fmt.Println(mac[:8])
            a = strings.Index(list[i], ")")
            company := list[i][a+1:]
            company = strings.TrimSpace(company)
            //fmt.Println(company)

            ouiMap[mac[:8]] = company

        }
    }
    if len(os.Args) < 2 {
        fmt.Println("Usage " + os.Args[0] + " <mac>")
        return
    }

    macAddress := os.Args[1]
    matched, err := regexp.MatchString("^([0-9A-Fa-f]{2}[:-]){2}([0-9A-Fa-f]{2})", macAddress[:8])
    if err != nil {
        fmt.Println("Not a MAC address")
    }

    if matched {
        elem, ok := ouiMap[macAddress[:8]]

        if ok {
            fmt.Println(elem)
        } else {
            fmt.Println("Manufacturer not found for MAC address: " + macAddress)

        }
    }

}
