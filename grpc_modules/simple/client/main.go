package main

import (
	"context"
	"log"
	pb "grpc_modules/simple/proto"
	"google.golang.org/grpc"
)

func main() {
	addr := "127.0.0.1:6868"

	conn, err := grpc.Dial(addr, grpc.WithInsecure())
	if err != nil {
		log.Fatalf("grpc client dial error:%v", err)
	}
	defer conn.Close()

	grpcClient := pb.NewUserServiceClient(conn)

	req := pb.UserRequest{ID:2}
	resp, err := grpcClient.GetUserInfo(context.Background(), &req)
	if err != nil {
		log.Fatalf("grpc client call error:%v", err)
	}
	log.Printf("grpc client receive res:%v\n", resp)
}