package main

import (
	"fmt"
	"io"
	"net"
	"sync"
	"time"
)

type Server struct {
	Ip        string
	Port      int
	OnlineMap map[string]*User
	mapLock   sync.RWMutex
	Message   chan string
}

//Create Server
func NewServer(ip string, port int) *Server {
	server := &Server{
		Ip:        ip,
		Port:      port,
		OnlineMap: make(map[string]*User),
		Message:   make(chan string),
	}
	return server
}

func (server *Server) ListenMessage() {
	for {
		msg := <-server.Message

		server.mapLock.Lock()
		for _, cli := range server.OnlineMap {
			cli.C <- msg
		}
		server.mapLock.Unlock()
	}
}

func (server *Server) BroadCast(user *User, msg string) {
	sendMsg := "[" + user.Name + "]:" + msg

	server.Message <- sendMsg
}

func (server *Server) Handler(conn net.Conn) {
	fmt.Println("new connection:", conn.RemoteAddr().String())

	user := NewUser(conn, server)

	user.Online()

	isLive := make(chan bool)

	//receive msg from client
	go func() {
		buf := make([]byte, 1024)
		for {
			n, err := conn.Read(buf)
			if 0 == n {
				user.Offline()
				fmt.Printf("user:%s, offline\n", user.Name)
				return
			}
			if err != nil && err != io.EOF {
				fmt.Println("read msg from client err:", err)
				return
			}

			msg := string(buf[:n-1])
			//
			user.DoMessage(msg)
			//
			isLive <- true
		}
	}()

	//
	for {
		select {
		case <-isLive:
			// the next code will be active(time.After)
		case <-time.After(time.Second * 500):
			user.SendMsg("you had be kicked!!!\n")
			close(user.C)
			conn.Close()
			return
		}
	}
}

//Start Server
func (server *Server) Start() {
	//socket listen
	listener, err := net.Listen("tcp", fmt.Sprintf("%s:%d", server.Ip, server.Port))
	if err != nil {
		fmt.Println("net listen err:", err)
		return
	}
	//close
	defer listener.Close()

	//listen msg
	go server.ListenMessage()

	for {
		//accept
		conn, err := listener.Accept()
		if err != nil {
			fmt.Println("net listener accept err:", err)
			continue
		}
		//handle
		go server.Handler(conn)
	}
}
