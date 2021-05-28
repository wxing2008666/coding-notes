// Code generated by protoc-gen-go. DO NOT EDIT.
// source: user.proto

/*
Package grpc_bidirectional_stream is a generated protocol buffer package.

It is generated from these files:
	user.proto

It has these top-level messages:
	UserRequest
	UserResponse
*/
package grpc_bidirectional_stream

import proto "github.com/golang/protobuf/proto"
import fmt "fmt"
import math "math"

import (
	context "golang.org/x/net/context"
	grpc "google.golang.org/grpc"
)

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = fmt.Errorf
var _ = math.Inf

// This is a compile-time assertion to ensure that this generated file
// is compatible with the proto package it is being compiled against.
// A compilation error at this line likely means your copy of the
// proto package needs to be updated.
const _ = proto.ProtoPackageIsVersion2 // please upgrade the proto package

type UserRequest struct {
	ID int32 `protobuf:"varint,1,opt,name=ID" json:"ID,omitempty"`
}

func (m *UserRequest) Reset()                    { *m = UserRequest{} }
func (m *UserRequest) String() string            { return proto.CompactTextString(m) }
func (*UserRequest) ProtoMessage()               {}
func (*UserRequest) Descriptor() ([]byte, []int) { return fileDescriptor0, []int{0} }

func (m *UserRequest) GetID() int32 {
	if m != nil {
		return m.ID
	}
	return 0
}

type UserResponse struct {
	Name string `protobuf:"bytes,1,opt,name=name" json:"name,omitempty"`
	Age  int32  `protobuf:"varint,2,opt,name=age" json:"age,omitempty"`
}

func (m *UserResponse) Reset()                    { *m = UserResponse{} }
func (m *UserResponse) String() string            { return proto.CompactTextString(m) }
func (*UserResponse) ProtoMessage()               {}
func (*UserResponse) Descriptor() ([]byte, []int) { return fileDescriptor0, []int{1} }

func (m *UserResponse) GetName() string {
	if m != nil {
		return m.Name
	}
	return ""
}

func (m *UserResponse) GetAge() int32 {
	if m != nil {
		return m.Age
	}
	return 0
}

func init() {
	proto.RegisterType((*UserRequest)(nil), "grpc.bidirectional.stream.UserRequest")
	proto.RegisterType((*UserResponse)(nil), "grpc.bidirectional.stream.UserResponse")
}

// Reference imports to suppress errors if they are not otherwise used.
var _ context.Context
var _ grpc.ClientConn

// This is a compile-time assertion to ensure that this generated file
// is compatible with the grpc package it is being compiled against.
const _ = grpc.SupportPackageIsVersion4

// Client API for UserService service

type UserServiceClient interface {
	GetUserInfo(ctx context.Context, opts ...grpc.CallOption) (UserService_GetUserInfoClient, error)
}

type userServiceClient struct {
	cc *grpc.ClientConn
}

func NewUserServiceClient(cc *grpc.ClientConn) UserServiceClient {
	return &userServiceClient{cc}
}

func (c *userServiceClient) GetUserInfo(ctx context.Context, opts ...grpc.CallOption) (UserService_GetUserInfoClient, error) {
	stream, err := grpc.NewClientStream(ctx, &_UserService_serviceDesc.Streams[0], c.cc, "/grpc.bidirectional.stream.UserService/GetUserInfo", opts...)
	if err != nil {
		return nil, err
	}
	x := &userServiceGetUserInfoClient{stream}
	return x, nil
}

type UserService_GetUserInfoClient interface {
	Send(*UserRequest) error
	Recv() (*UserResponse, error)
	grpc.ClientStream
}

type userServiceGetUserInfoClient struct {
	grpc.ClientStream
}

func (x *userServiceGetUserInfoClient) Send(m *UserRequest) error {
	return x.ClientStream.SendMsg(m)
}

func (x *userServiceGetUserInfoClient) Recv() (*UserResponse, error) {
	m := new(UserResponse)
	if err := x.ClientStream.RecvMsg(m); err != nil {
		return nil, err
	}
	return m, nil
}

// Server API for UserService service

type UserServiceServer interface {
	GetUserInfo(UserService_GetUserInfoServer) error
}

func RegisterUserServiceServer(s *grpc.Server, srv UserServiceServer) {
	s.RegisterService(&_UserService_serviceDesc, srv)
}

func _UserService_GetUserInfo_Handler(srv interface{}, stream grpc.ServerStream) error {
	return srv.(UserServiceServer).GetUserInfo(&userServiceGetUserInfoServer{stream})
}

type UserService_GetUserInfoServer interface {
	Send(*UserResponse) error
	Recv() (*UserRequest, error)
	grpc.ServerStream
}

type userServiceGetUserInfoServer struct {
	grpc.ServerStream
}

func (x *userServiceGetUserInfoServer) Send(m *UserResponse) error {
	return x.ServerStream.SendMsg(m)
}

func (x *userServiceGetUserInfoServer) Recv() (*UserRequest, error) {
	m := new(UserRequest)
	if err := x.ServerStream.RecvMsg(m); err != nil {
		return nil, err
	}
	return m, nil
}

var _UserService_serviceDesc = grpc.ServiceDesc{
	ServiceName: "grpc.bidirectional.stream.UserService",
	HandlerType: (*UserServiceServer)(nil),
	Methods:     []grpc.MethodDesc{},
	Streams: []grpc.StreamDesc{
		{
			StreamName:    "GetUserInfo",
			Handler:       _UserService_GetUserInfo_Handler,
			ServerStreams: true,
			ClientStreams: true,
		},
	},
	Metadata: "user.proto",
}

func init() { proto.RegisterFile("user.proto", fileDescriptor0) }

var fileDescriptor0 = []byte{
	// 181 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0xe2, 0xe2, 0x2a, 0x2d, 0x4e, 0x2d,
	0xd2, 0x2b, 0x28, 0xca, 0x2f, 0xc9, 0x17, 0x92, 0x4c, 0x2f, 0x2a, 0x48, 0xd6, 0x4b, 0xca, 0x4c,
	0xc9, 0x2c, 0x4a, 0x4d, 0x2e, 0xc9, 0xcc, 0xcf, 0x4b, 0xcc, 0xd1, 0x2b, 0x2e, 0x29, 0x4a, 0x4d,
	0xcc, 0x55, 0x92, 0xe5, 0xe2, 0x0e, 0x2d, 0x4e, 0x2d, 0x0a, 0x4a, 0x2d, 0x2c, 0x4d, 0x2d, 0x2e,
	0x11, 0xe2, 0xe3, 0x62, 0xf2, 0x74, 0x91, 0x60, 0x54, 0x60, 0xd4, 0x60, 0x0d, 0x62, 0xf2, 0x74,
	0x51, 0x32, 0xe1, 0xe2, 0x81, 0x48, 0x17, 0x17, 0xe4, 0xe7, 0x15, 0xa7, 0x0a, 0x09, 0x71, 0xb1,
	0xe4, 0x25, 0xe6, 0xa6, 0x82, 0x55, 0x70, 0x06, 0x81, 0xd9, 0x42, 0x02, 0x5c, 0xcc, 0x89, 0xe9,
	0xa9, 0x12, 0x4c, 0x60, 0x4d, 0x20, 0xa6, 0x51, 0x21, 0xc4, 0xd0, 0xe0, 0xd4, 0xa2, 0xb2, 0xcc,
	0xe4, 0x54, 0xa1, 0x24, 0x2e, 0x6e, 0xf7, 0xd4, 0x12, 0x90, 0x88, 0x67, 0x5e, 0x5a, 0xbe, 0x90,
	0x9a, 0x1e, 0x4e, 0xe7, 0xe8, 0x21, 0xb9, 0x45, 0x4a, 0x9d, 0xa0, 0x3a, 0x88, 0xa3, 0x34, 0x18,
	0x0d, 0x18, 0x93, 0xd8, 0xc0, 0x3e, 0x35, 0x06, 0x04, 0x00, 0x00, 0xff, 0xff, 0x0c, 0x40, 0x7f,
	0xe7, 0xf7, 0x00, 0x00, 0x00,
}
