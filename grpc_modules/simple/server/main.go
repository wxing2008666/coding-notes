package main

import (
	"context"
	pb "grpc_modules/simple/proto"
	"net"
	"log"
	"google.golang.org/grpc"
)

var users = map[int32]pb.UserResponse{
	1: {Name: "wang", Age: 20},
	2: {Name: "li", Age: 21},
	3: {Name: "zhao", Age: 22},
}

type simpleServer struct{}

func (s *simpleServer) GetUserInfo(ctx context.Context, req *pb.UserRequest) (resp *pb.UserResponse, err error) {
	if user, ok := users[req.ID]; ok {
		resp = &user
	}
	log.Printf("received req:%v\n", req)
	return
}

func main() {
	addr := "127.0.0.1:6868"
	listener, err := net.Listen("tcp", addr)
	if err != nil {
		log.Fatalf("listen error:%v", err)
	} else {
		log.Println("server listen at:", addr)
	}

	grpcserver := grpc.NewServer()

	pb.RegisterUserServiceServer(grpcserver, &simpleServer{})

	grpcserver.Serve(listener)
}