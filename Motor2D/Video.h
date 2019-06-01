#ifndef _VIDEO_H_
#define _VIDEO_H_

struct SDL_Texture;
struct AVFormatContext;
struct AVFrame;
struct AVCodecContext;
struct AVStream;
struct SwsContext;
struct SwrContext;
struct AVPacket;
struct AVPacketList;

#include "j1Module.h"

struct SDL_Mutex;
struct SDL_cond;
struct SDL_Thread;

struct PacketQueue {
	AVPacketList *first_pkt, *last_pkt;
	int nb_packets;
	int size;
	SDL_mutex* mutex = nullptr;
	SDL_cond* cond = nullptr;
	bool paused = false;

	void Init();
	int PutPacket(AVPacket* pkt);
	int GetPacket(AVPacket* pkt, bool block);
	int Clear();
};

struct StreamComponent
{
	AVStream* stream = nullptr;
	AVCodecContext* context = nullptr;
	AVFrame* frame = nullptr;
	AVFrame* converted_frame = nullptr;
	PacketQueue pktqueue;

	double clock = 0.0;
	int stream_index = 0;
	bool finished = false;

	void Clear();
};

class Video : public j1Module
{
public:
	Video();
	~Video();
	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	int PlayVideo(std::string file_path);
	bool Pause();
	void CloseVideo();


	int DecodeAudio();

private:
	void OpenStreamComponent(int stream_index);
	void DecodeVideo();

public:
	StreamComponent audio;
	StreamComponent video;
	AVFormatContext* format = nullptr;

	bool refresh = false;
	bool playing = false;
	bool paused = false;
	bool quit = false;
	std::string file = "";

	//Audio stream stuff, this should probably not be here...
	uint8_t audio_buf[(192000 * 3) / 2]; //buffer where we store the audio data
	unsigned int audio_buf_size = 0;
	unsigned int audio_buf_index = 0;

private:
	SDL_Texture* texture = nullptr;

	SwsContext* sws_context = nullptr;
	SwrContext* swr_context = nullptr;

	SDL_Thread* parse_thread_id;
};

#endif
