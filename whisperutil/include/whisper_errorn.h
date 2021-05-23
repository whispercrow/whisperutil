#ifndef WHISPER_ERRORN_H
#define WHISPER_ERRORN_H

#define WHISPER_ERR_SUCCESS                 0x00000000			//成功
#define WHISPER_ERR_PARAM					0xe0001001			//参数错误
#define WHISPER_ERR_STRFORMAT				0xe0001002			//字符串格式不正确
#define WHISPER_ERR_SMALLBUFFER             0xe0001003			//缓冲区太小
#define WHISPER_ERR_MEMINSUFFICIENT         0xe0001004			//内存不足
#define WHISPER_ERR_GETPATH                 0xe0001005			//获取路径失败
#define WHISPER_ERR_MODULE                  0xe0001006			//模块加载失败
#define WHISPER_ERR_FUNCTION				0xe0001007			//函数指针为空
#define WHISPER_ERR_TIMEOUT					0xe0001008			//动作超时

#endif // WHISPER_ERRORN_H
