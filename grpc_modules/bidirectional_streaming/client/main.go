package main

import (
	"log"
	"context"
	"time"
	pb "grpc_modules/bidirectional_streaming/proto"
	"google.golang.org/grpc"
)

func main() {
	addr := "0.0.0.0:6868"
	conn, err := grpc.Dial(addr, grpc.WithInsecure())
	if err != nil {
		log.Fatalf("grpc client error:%v", err)
	}
	defer conn.Close()

	grpcClient := pb.NewUserServiceClient(conn)

	stream, err := grpcClient.GetUserInfo(context.Background())
	if err != nil {
		log.Fatalf("grpc client get user info error:%v", err)
	}

	var i int32
	for i = 1; i < 4; i++ {
		stream.Send(&pb.UserRequest{ID:i})
		time.Sleep(1 * time.Second)

		resp, err := stream.Recv()
		if err != nil {
			log.Fatalf("grpc client recv error:%v", err)
		}
		log.Printf("[RECEIVED RESPONSE]: %v\n", resp) // 输出哝应
	}
}