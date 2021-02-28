package main

import (
	"flag"
	"fmt"
	"io"
	"net"
	"os"
)

type Client struct {
	ServerIp   string
	ServerPort int
	conn       net.Conn
	Name       string
	flag       int
}

func NewClient(serverIp string, serverPort int) *Client {
	client := &Client{
		ServerIp:   serverIp,
		ServerPort: serverPort,
		flag:       100,
	}

	conn, err := net.Dial("tcp", fmt.Sprintf("%s:%d", serverIp, serverPort))
	if err != nil {
		fmt.Println("net dial err:", err)
		return nil
	}

	client.conn = conn
	return client
}

func (client *Client) menu() bool {
	var flag int

	fmt.Println("please input flag:")
	fmt.Println("1: p2p im")
	fmt.Println("2: broadcast im")
	fmt.Println("3: rename")
	fmt.Println("0: exit")

	fmt.Scanln(&flag)
	if flag < 0 || flag > 3 {
		fmt.Println("input flag is not support!")
		return false
	}

	client.flag = flag
	return true
}

func (client *Client) SelectUser() {
	msg := "who\n"
	_, err := client.conn.Write([]byte(msg))
	if err != nil {
		fmt.Println("select use failed")
	}
}

func (client *Client) P2PMsg() {
	client.SelectUser()
	fmt.Println("please select name(exit to quit)")

	var remoteName string
	fmt.Scanln(&remoteName)

	for remoteName != "exit" {
		fmt.Println("please input msg(exit to quit)")

		var msg string
		fmt.Scanln(&msg)
		for msg != "exit" {
			sendMsg := "to|" + remoteName + "|" + msg + "\n\n"
			_, err := client.conn.Write([]byte(sendMsg))
			if err != nil {
				fmt.Println("P2PMsg send msg failed")
			}
			//
			msg = ""
			fmt.Println("please input msg(exit to quit)")
			fmt.Scanln(&msg)
		}
		//
		remoteName = ""
		fmt.Println("please select name(exit to quit)")
		fmt.Scanln(&remoteName)
	}
}

func (client *Client) BroadCastMsg() {
	fmt.Println("please input msg, if msg is exit then quit")

	var msg string
	fmt.Scanln(&msg)

	for msg != "exit" {
		if len(msg) != 0 {
			sendMsg := msg + "\n"
			_, err := client.conn.Write([]byte(sendMsg))
			if err != nil {
				fmt.Println("BroadCastMsg send msg failed")
				break
			}
		}
		//
		msg = ""
		fmt.Println("please input msg, if msg is exit then quit")
		fmt.Scanln(&msg)
	}
}

func (client *Client) Rename() bool {
	fmt.Println("please input the name you will update")

	var newName string
	fmt.Scanln(&newName)
	//
	sendMsg := "rename|" + newName + "\n"
	_, err := client.conn.Write([]byte(sendMsg))
	if err != nil {
		fmt.Println("rename failed")
		return false
	}
	return true
}

func (client *Client) HandleResponse() {
	io.Copy(os.Stdout, client.conn)
}

func (client *Client) Run() {
	for client.flag != 0 {
		for client.menu() != true {

		}

		switch client.flag {
		case 1:
			client.P2PMsg()
			break
		case 2:
			client.BroadCastMsg()
			break
		case 3:
			client.Rename()
			break
		}
	}
}

var ServerIp string
var ServerPort int

// ./client -ip 127.0.0.1 -port 8080
func init() {
	flag.StringVar(&ServerIp, "ip", "127.0.0.1", "server ip will be connected(default:127.0.0.1)")
	flag.IntVar(&ServerPort, "port", 8080, "the server port to be connected(default:8080)")
}

func main() {
	flag.Parse()

	client := NewClient(ServerIp, ServerPort)
	if client == nil {
		fmt.Println("client connect failed")
		return
	}
	fmt.Println("client connect success")

	go client.HandleResponse()

	client.Run()
}
