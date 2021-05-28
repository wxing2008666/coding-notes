package main

import (
	"log"
	"context"
	pb "grpc_modules/client_side_streaming/proto"
	"google.golang.org/grpc"
)

func main() {
	addr := "0.0.0.0:6868"
	conn, err := grpc.Dial(addr, grpc.WithInsecure())
	if err != nil {
		log.Fatalf("grpc client dial error:%v", err)
	}
	defer conn.Close()

	grpcClient := pb.NewUserServiceClient(conn)

	stream, err := grpcClient.GetUserInfo(context.Background())
	if err != nil {
		log.Fatalf("grpc client get user info error:%v", err)
	}

	var i int32
	for i = 1; i < 4; i++ {
		err := stream.Send(&pb.UserRequest{ID:i})
		if err != nil {
			log.Fatalf("grpc client send error:%v", err)
		}
	}

	resp, err := stream.CloseAndRecv()
	if err != nil {
		log.Fatalf("grpc client recv error:%v", err)
	}
	log.Printf("resp from server:%v", resp)
}