package main

import (
	"crypto/sha1"
	"encoding/base64"
	"encoding/binary"
	"io"
	"log"
	"net"
	"strings"
	"time"
)

type WsSocket struct {
	Conn net.Conn
}

// 帧类型(OPCODE). RFC 6455, section 11.8.
const (
	FRAME_CONTINUE = 0  //继续帧
	FRAME_TEXT     = 1  //文本帧
	FRAME_BINARY   = 2  //二进制帧
	FRAME_CLOSE    = 8  //关闭帧
	FRAME_PING     = 9  //ping帧
	FRAME_PONG     = 10 //pong帧
)

func init() {
	//初始化日志打印格式
	log.SetFlags(log.Lshortfile | log.LstdFlags)
}

func main() {
	ln, err := net.Listen("tcp", "10.25.170.33:8000")
	defer ln.Close()
	if err != nil {
		log.Panic(err)
	}

	for {
		conn, err := ln.Accept()
		if err != nil {
			log.Println("accept err:", err)
		}
		go handleConnection(conn)
	}

}

func handleConnection(conn net.Conn) {
	// http request open websocket
	content := make([]byte, 1024)
	conn.Read(content)
	log.Printf("http request:\n%s\n", string(content))
	headers := parseHttpHeader(string(content))
	secWebsocketKey := headers["Sec-WebSocket-Key"]

	// http response open websocket
	response := "HTTP/1.1 101 Switching Protocols\r\n"
	response += "Sec-WebSocket-Accept: " + computeAcceptKey(secWebsocketKey) + "\r\n"
	response += "Connection: Upgrade\r\n"
	response += "Upgrade: websocket\r\n\r\n"
	log.Printf("http response:\n%s\n", response)
	if lenth, err := conn.Write([]byte(response)); err != nil {
		log.Println(err)
	} else {
		log.Println("send http response len:", lenth)
	}

	// websocket established
	wssocket := &WsSocket{Conn: conn}

	//begin test case
	for {
		time.Sleep(5 * time.Second)
		// frame ping
		wssocket.SendIframe(FRAME_PING, []byte("hello"))
		// frame text
		//wssocket.SendIframe(FRAME_TEXT, []byte("hello"))
		// frame read //浏览器响应同样负载数据的pong帧
		log.Printf("server read data from client:\n%s\n", string(wssocket.ReadIframe()))
	}
	//end test case
}

//发送帧给客户端(不考虑分片)(只做服务端,无掩码)
func (this *WsSocket) SendIframe(OPCODE int, frameData []byte) {
	dataLen := len(frameData)
	var n int
	var err error

	//第一个字节b1
	b1 := 0x80 | byte(OPCODE)
	n, err = this.Conn.Write([]byte{b1})
	if err != nil {
		log.Printf("Conn.Write() error,length:%d;error:%s\n", n, err)
		if err == io.EOF {
			log.Println("客户端已经断开WsSocket!")
		} else if err.(*net.OpError).Err.Error() == "use of closed network connection" {
			log.Println("服务端已经断开WsSocket!")
		}
	}

	//第二个字节
	var b2 byte
	var payloadLen int
	switch {
	case dataLen <= 125:
		b2 = byte(dataLen)
		payloadLen = dataLen
	case 126 <= dataLen && dataLen <= 65535:
		b2 = byte(126)
		payloadLen = 126
	case dataLen > 65535:
		b2 = byte(127)
		payloadLen = 127
	}
	this.Conn.Write([]byte{b2})

	//如果payloadLen不够用,写入exPayLenByte,用exPayLenByte表示负载数据的长度
	switch payloadLen {
	case 126:
		exPayloadLenByte := make([]byte, 2)
		exPayloadLenByte[0] = byte(dataLen >> 8) //高8位
		exPayloadLenByte[1] = byte(dataLen)      //低8位
		this.Conn.Write(exPayloadLenByte)        //扩展2个字节表示负载数据长度, 最高位也可以用
	case 127:
		exPayloadLenByte := make([]byte, 8)
		exPayloadLenByte[0] = byte(dataLen >> 56) //第1个字节
		exPayloadLenByte[1] = byte(dataLen >> 48) //第2个字节
		exPayloadLenByte[2] = byte(dataLen >> 40) //第3个字节
		exPayloadLenByte[3] = byte(dataLen >> 32) //第4个字节
		exPayloadLenByte[4] = byte(dataLen >> 24) //第5个字节
		exPayloadLenByte[5] = byte(dataLen >> 16) //第6个字节
		exPayloadLenByte[6] = byte(dataLen >> 8)  //第7个字节
		exPayloadLenByte[7] = byte(dataLen)       //第8个字节
		this.Conn.Write(exPayloadLenByte)         //扩展8个字节表示负载数据长度, 最高位不可以用,必须为0
	}
	this.Conn.Write(frameData) //无掩码,直接在表示长度的区域后面写入数据
	log.Printf("real payloadLen=%d:该数据帧的真实负载数据长度(bytes).\n", dataLen)
	log.Println("MASK=0:没有掩码.")
	log.Printf("server send data to client:\n%s\n", string(frameData))

}

//读取客户端发送的帧(考虑分片)
func (this *WsSocket) ReadIframe() (frameData []byte) {
	var n int
	var err error

	//第一个字节
	b1 := make([]byte, 1)
	n, err = this.Conn.Read(b1)
	if err != nil {
		log.Printf("Conn.Read() error,length:%d;error:%s\n", n, err)
		if err == io.EOF {
			log.Println("客户端已经断开WsSocket!")
		} else if err.(*net.OpError).Err.Error() == "use of closed network connection" {
			log.Println("服务端已经断开WsSocket!")
		}
	}
	FIN := b1[0] >> 7
	OPCODE := b1[0] & 0x0F
	if OPCODE == 8 {
		log.Println("OPCODE=8:连接关闭帧.")
		this.SendIframe(FRAME_CLOSE, formatCloseMessage(1000, "因为收到客户端的主动关闭请求,所以响应."))
		this.Conn.Close()
		return
	}

	//第二个字节
	b2 := make([]byte, 1)
	this.Conn.Read(b2)
	payloadLen := int64(b2[0] & 0x7F) //payloadLen:表示数据报文长度(可能不够用),0x7F(16) > 01111111(2)
	MASK := b2[0] >> 7                //MASK=1:表示客户端发来的数据,且表示采用了掩码(客户端传来的数据必须采用掩码)
	log.Printf("second byte:MASK=%d, raw payloadLen=%d\n", MASK, payloadLen)

	//扩展长度
	dataLen := payloadLen
	switch {
	case payloadLen == 126:
		// 如果payloadLen=126,启用2个字节作为拓展,表示更长的报文
		// 负载数据的长度范围(bytes):126~65535(2) 0xffff
		log.Println("raw payloadLen=126,启用2个字节作为拓展（最高有效位可以是1,使用所有位）,表示更长的报文")
		exPayloadLenByte := make([]byte, 2)
		n, err := this.Conn.Read(exPayloadLenByte)
		if err != nil {
			log.Printf("Conn.Read() error,length:%d;error:%s\n", n, err)
		}
		dataLen = int64(exPayloadLenByte[0])<<8 + int64(exPayloadLenByte[1])

	case payloadLen == 127:
		// 如果payloadLen=127,启用8个字节作为拓展,表示更长的报文
		// 负载数据的长度范围(bytes):65536~0x7fff ffff ffff ffff
		log.Println("payloadLen=127,启用8个字节作为拓展（最高有效位必须是0,舍弃最高位）,表示更长的报文")
		exPayloadLenByte := make([]byte, 8)
		this.Conn.Read(exPayloadLenByte)
		dataLen = int64(exPayloadLenByte[0])<<56 + int64(exPayloadLenByte[1])<<48 + int64(exPayloadLenByte[2])<<40 + int64(exPayloadLenByte[3])<<32 + int64(exPayloadLenByte[4])<<24 + int64(exPayloadLenByte[5])<<16 + int64(exPayloadLenByte[6])<<8 + int64(exPayloadLenByte[7])
	}
	log.Printf("real payloadLen=%d:该数据帧的真实负载数据长度(bytes).\n", dataLen)

	//掩码
	maskingByte := make([]byte, 4)
	if MASK == 1 {
		this.Conn.Read(maskingByte)
		log.Println("MASK=1:负载数据采用了掩码.")
	} else if MASK == 0 {
		log.Println("MASK=0:没有掩码.")
	}

	//数据
	payloadDataByte := make([]byte, dataLen)
	this.Conn.Read(payloadDataByte)
	dataByte := make([]byte, dataLen)
	for i := int64(0); i < dataLen; i++ {
		if MASK == 1 { //解析掩码数据
			dataByte[i] = payloadDataByte[i] ^ maskingByte[i%4]
		} else {
			dataByte[i] = payloadDataByte[i]
		}
	}

	//如果没有数据,强制停止递归
	if dataLen <= 0 {
		return
	}
	//最后一帧,正常停止递归
	if FIN == 1 {
		return dataByte
	}
	//中间帧
	nextData := this.ReadIframe()
	//汇总
	return append(frameData, nextData...)
}

//计算Sec-WebSocket-Accept
func computeAcceptKey(secWebsocketKey string) string {
	var keyGUID = []byte("258EAFA5-E914-47DA-95CA-C5AB0DC85B11")
	h := sha1.New()
	h.Write([]byte(secWebsocketKey))
	h.Write(keyGUID)
	return base64.StdEncoding.EncodeToString(h.Sum(nil))
}

//HTTP报文头部map
func parseHttpHeader(content string) map[string]string {
	headers := make(map[string]string, 10)
	lines := strings.Split(content, "\r\n")
	for _, line := range lines {
		if len(line) >= 0 {
			words := strings.Split(line, ":")
			if len(words) == 2 {
				headers[strings.Trim(words[0], " ")] = strings.Trim(words[1], " ")
			}
		}
	}
	return headers
}

//二进制逐位打印字节数组
func printBinary(data []byte) {
	for i := 0; i < len(data); i++ {
		byteData := data[i]
		var j uint
		for j = 7; j > 0; j-- {
			log.Printf("%d", ((byteData >> j) & 0x01))
		}
		log.Printf("%d\n", ((byteData >> j) & 0x01))
	}
}

//关闭码 + 关闭原因 = 关闭帧的负载数据
func formatCloseMessage(closeCode int, text string) []byte {
	buf := make([]byte, 2+len(text))
	binary.BigEndian.PutUint16(buf, uint16(closeCode))
	copy(buf[2:], text)
	return buf
}

