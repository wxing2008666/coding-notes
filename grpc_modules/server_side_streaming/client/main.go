package main

import (
	"log"
	"io"
	"context"
	pb "grpc_modules/server_side_streaming/proto"
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

	req := pb.UserRequest{ID:3}
	stream, err := grpcClient.GetUserInfo(context.Background(), &req)
	if err != nil {
		log.Fatalf("grpc client get user info error:%v", err)
	}

	for {
		resp, err := stream.Recv()
		if err == io.EOF {
			log.Println("receive from server complete")
			break
		}
		if err != nil {
			log.Fatalf("grpc client stream recv error:%v", err)
		}
		log.Printf("resp form server:%v", resp)
	}
}