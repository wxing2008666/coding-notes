package main

import (
	"log"
	"net"
	pb "grpc_modules/server_side_streaming/proto"
	"google.golang.org/grpc"
)

var users = map[int32]pb.UserResponse{
	1: {Name: "wang", Age: 18},
	2: {Name: "li", Age: 19},
	3: {Name: "zhao", Age: 20},
}

type serverSideStreamServer struct{}

func (s *serverSideStreamServer) GetUserInfo(req *pb.UserRequest, stream pb.UserService_GetUserInfoServer) error {
	for _, user := range users {
		stream.Send(&user)
	}
	log.Printf("receive req:%v", req)
	return nil
}

func main() {
	addr := "0.0.0.0:6868"
	listener, err := net.Listen("tcp", addr)
	if err != nil {
		log.Fatalf("grpc server error:%v", err)
	} else {
		log.Printf("grpc server listen at:%v", addr)
	}

	grpcserver := grpc.NewServer()

	pb.RegisterUserServiceServer(grpcserver, &serverSideStreamServer{})

	grpcserver.Serve(listener)
}