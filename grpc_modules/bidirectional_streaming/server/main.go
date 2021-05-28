package main

import (
	"log"
	"net"
	"io"
	pb "grpc_modules/bidirectional_streaming/proto"
	"google.golang.org/grpc"
)

var users = map[int32]pb.UserResponse{
	1: {Name: "wang", Age: 18},
	2: {Name: "li", Age: 19},
	3: {Name: "zhao", Age: 20},
}

type bidirectionalStreamServer struct{}

func (b *bidirectionalStreamServer) GetUserInfo(stream pb.UserService_GetUserInfoServer) error {
	for {
		req, err := stream.Recv()
		if err == io.EOF {
			return nil
		}
		if err != nil {
			return err
		}
		u := users[req.ID]
		err = stream.Send(&u)
		if err != nil {
			return err
		}
		log.Printf("[RECEVIED REQUEST]: %v\n", req)
	}
	return nil
}

func main() {
	addr := "0.0.0.0:6868"
	listener, err := net.Listen("tcp", addr)
	if err != nil {
		log.Fatalf("grpc server listen error:%v\n", err)
	} else {
		log.Printf("grpc server listen at:%v", addr)
	}

	grpcserver := grpc.NewServer()

	pb.RegisterUserServiceServer(grpcserver, &bidirectionalStreamServer{})

	grpcserver.Serve(listener)
}