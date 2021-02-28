package main

import (
	"net"
	"strings"
)

type User struct {
	Name   string
	Addr   string
	C      chan string
	conn   net.Conn
	server *Server
}

func NewUser(conn net.Conn, server *Server) *User {
	addr := conn.RemoteAddr().String()
	user := &User{
		Name:   addr,
		Addr:   addr,
		C:      make(chan string),
		conn:   conn,
		server: server,
	}

	//listen msg
	go user.ListenMessage()

	return user
}

func (user *User) Online() {
	user.server.mapLock.Lock()
	user.server.OnlineMap[user.Name] = user
	user.server.mapLock.Unlock()
	//user online broadcast
	user.server.BroadCast(user, ", online now")
}

func (user *User) Offline() {
	user.server.mapLock.Lock()
	delete(user.server.OnlineMap, user.Name)
	user.server.mapLock.Unlock()
	//user online broadcast
	user.server.BroadCast(user, ", offline")
}

func (user *User) SendMsg(msg string) {
	user.conn.Write([]byte(msg))
}

func (user *User) DoMessage(msg string) {
	if "who" == msg {
		user.server.mapLock.Lock()
		for _, usr := range user.server.OnlineMap {
			sendMsg := "[" + usr.Name + "]\n"
			user.SendMsg(sendMsg)
		}
		user.server.mapLock.Unlock()
	} else if len(msg) > 7 && msg[:7] == "rename|" {
		//format: rename|aaa
		newName := strings.Split(msg, "|")[1]

		_, ok := user.server.OnlineMap[newName]
		if ok {
			user.SendMsg("name already exists, can not rename\n")
		} else {
			user.server.mapLock.Lock()
			delete(user.server.OnlineMap, user.Name)
			user.server.OnlineMap[newName] = user
			user.server.mapLock.Unlock()
			//
			user.Name = newName
			user.SendMsg("you had rename to:" + newName + "\n")
		}
	} else if len(msg) > 3 && msg[:3] == "to|" {
		//format: to|123|msg
		remoteName := strings.Split(msg, "|")[1]
		if remoteName == "" {
			user.SendMsg("the user is wrong, to use like \"to|abc|msg\"\n")
			return
		}
		//
		remoteUser, ok := user.server.OnlineMap[remoteName]
		if !ok {
			user.SendMsg("the user not exists\n")
			return
		}
		content := strings.Split(msg, "|")[2]
		if content == "" {
			user.SendMsg("msg is empty, can not send\n")
			return
		}
		remoteUser.SendMsg(user.Name + " send msg for you:" + content + "\n")
	} else {
		user.server.BroadCast(user, msg)
	}
}

func (user *User) ListenMessage() {
	for {
		msg := <-user.C
		//
		user.conn.Write([]byte(msg + "\n"))
	}
}
