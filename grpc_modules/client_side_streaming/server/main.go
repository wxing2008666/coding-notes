package main

import (
	"log"
	"net"
	"io"
	pb "grpc_modules/client_side_streaming/proto"
	"google.golang.org/grpc"
)

var users = map[int32]pb.UserResponse{
	1: {Name: "wang", Age: 18},
	2: {Name: "li", Age: 19},
	3: {Name: "zhao", Age: 20},
}

type clientSideStreamServer struct{}

func (c *clientSideStreamServer) GetUserInfo(stream pb.UserService_GetUserInfoServer) error {
	var lastID int32
	for {
		req, err := stream.Recv()
		if err == io.EOF {
			if resp, ok := users[lastID]; ok {
				stream.SendAndClose(&resp)
				return nil
			}
		}
		lastID = req.ID
		log.Printf("receive req:%v\n", req)
	}
	return nil
}

func main() {
	addr := "0.0.0.0:6868"
	listener, err := net.Listen("tcp", addr)
	if err != nil {
		log.Fatalf("grpc server listen error:%v", err)
	} else {
		log.Printf("grpc server listen at:%v", addr)
	}

	grpcserver := grpc.NewServer()

	pb.RegisterUserServiceServer(grpcserver, &clientSideStreamServer{})

	grpcserver.Serve(listener)
}