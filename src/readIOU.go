package main

import (
    "fmt"
    "io/ioutil"
    "log"
    "os"
    "regexp"
    "strings"
)

type Result struct {
    ouiMap map[string]string
    err    error
}

//var ouiMap map[string]string

func readOUIFile(channel chan (Result)) {
    ouiMap := make(map[string]string)

    res := new(Result)
    file, err := ioutil.ReadFile("../oui.txt")
    if err != nil {
        //log.Fatal(err)
        res.err = err
        channel <- *res

        //return nil, err
    }

    //fmt.Println(file) // print the content as 'bytes'

    str := string(file) // convert content to a 'string'

    list := strings.SplitAfter(str, "\n")

    for _, str := range list {

        //fmt.Println(str)
        if strings.Contains(str, "(hex)") {

            a := strings.Index(str, "(")
            mac := str[:a]
            mac = strings.TrimSpace(mac)
            //fmt.Println(mac[:8])
            a = strings.Index(str, ")")
            company := str[a+1:]
            company = strings.TrimSpace(company)
            //fmt.Println(company)

            ouiMap[mac[:8]] = company

        }
    }

    res.ouiMap = ouiMap
    res.err = err
    channel <- *res // send sum to c
    //return ouiMap, err
}

func main() {

    //ouiMap, err :=
    res := new(Result)

    channel := make(chan Result)

    go readOUIFile(channel)

    *res = <-channel

    //fmt.Println(ouiMap)
    //, <-channel // receive from c

    if res.err != nil {
        fmt.Println("Error reading OUI file")
        log.Fatal(res.err)
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

        //macAddress =
        //macAddress =
        elem, ok := res.ouiMap[strings.Replace(strings.ToUpper(macAddress), ":", "-", -1)[:8]]

        if ok {
            fmt.Println(elem)
        } else {
            fmt.Println("Manufacturer not found for MAC address: " + macAddress)

        }
    }

}
