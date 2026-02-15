#include <cstdint>
#include <Windows.h>
#include "bink.hpp"
#include "../target.hpp"

struct BINK;
HMODULE H_BINKW32;

void* BINK_CLOSE;
void* BINK_COPY_TO_BUFFER_RECT;
void* BINK_DO_FRAME;
void* BINK_GET_RECTS;
void* BINK_NEXT_FRAME;
void* BINK_OPEN;
void* BINK_OPEN_DIRECTSOUND;
void* BINK_PAUSE;
void* BINK_SET_SOUND_SYSTEM;
void* BINK_SET_SOUND_TRACK;
void* BINK_SET_VOLUME;
void* BINK_SHOULD_SKIP;
void* BINK_WAIT;

// Used by TVG, present in MN's binkw32.
void* BINK_GO_TO;
void* BINK_COPY_TO_BUFFER;

// Used by 2TVG, present in MN's binkw32.
void* BINK_GET_FRAME_BUFFERS_INFO;
void* BINK_GET_SUMMARY;
void* BINK_REGISTER_FRAME_BUFFERS;
void* BINK_SET_MEMORY;

// Used by 2TVG, but not present in MN's binkw32.
void* BINK_SET_SPEAKER_VOLUMES;
void* BINK_GET_TRACK_ID;

// Used by 2TVGA
void* BINK_SET_PAN;

// Used by DI3G
void* BINK_START_ASYNC_THREAD;
void* BINK_SET_OS_FILE_CALLBACKS;
void* BINK_SET_WILL_LOOP;
void* BINK_DO_FRAME_ASYNC;
void* BINK_DO_FRAME_ASYNC_WAIT;
void* BINK_OPEN_XAUDIO2;

// TCS
void* BINK_GET_REALTIME;

void bink::replace_funcs() {
	H_BINKW32 = LoadLibraryA("binkw32.dll");
	if (H_BINKW32 == nullptr) {
		H_BINKW32 = LoadLibraryA("bink2w32.dll");
	}
	if (H_BINKW32 == nullptr) {
		H_BINKW32 = LoadLibraryA("bink2winrt_x86.uni10.dll");
	}

	if (H_BINKW32 != nullptr) {

		BINK_CLOSE = GetProcAddress(H_BINKW32, "_BinkClose@4");
		BINK_COPY_TO_BUFFER_RECT = GetProcAddress(H_BINKW32, "_BinkCopyToBufferRect@44");
		BINK_DO_FRAME = GetProcAddress(H_BINKW32, "_BinkDoFrame@4");
		BINK_GET_RECTS = GetProcAddress(H_BINKW32, "_BinkGetRects@8");
		BINK_NEXT_FRAME = GetProcAddress(H_BINKW32, "_BinkNextFrame@4");
		BINK_OPEN = GetProcAddress(H_BINKW32, "_BinkOpen@8");
		BINK_OPEN_DIRECTSOUND = GetProcAddress(H_BINKW32, "_BinkOpenDirectSound@4");
		BINK_PAUSE = GetProcAddress(H_BINKW32, "_BinkPause@8");
		BINK_SET_SOUND_SYSTEM = GetProcAddress(H_BINKW32, "_BinkSetSoundSystem@8");
		BINK_SET_SOUND_TRACK = GetProcAddress(H_BINKW32, "_BinkSetSoundTrack@8");
		BINK_SET_VOLUME = GetProcAddress(H_BINKW32, "_BinkSetVolume@12");
		BINK_SHOULD_SKIP = GetProcAddress(H_BINKW32, "_BinkShouldSkip@4");
		BINK_WAIT = GetProcAddress(H_BINKW32, "_BinkWait@4");

		BINK_GO_TO = GetProcAddress(H_BINKW32, "_BinkGoto@12");
		BINK_COPY_TO_BUFFER = GetProcAddress(H_BINKW32, "_BinkCopyToBuffer@28");

		BINK_GET_FRAME_BUFFERS_INFO = GetProcAddress(H_BINKW32, "_BinkGetFrameBuffersInfo@8");
		BINK_GET_SUMMARY = GetProcAddress(H_BINKW32, "_BinkGetSummary@8");
		BINK_REGISTER_FRAME_BUFFERS = GetProcAddress(H_BINKW32, "_BinkRegisterFrameBuffers@8");
		BINK_SET_MEMORY = GetProcAddress(H_BINKW32, "_BinkSetMemory@8");

		BINK_SET_SPEAKER_VOLUMES = GetProcAddress(H_BINKW32, "_BinkSetSpeakerVolumes@20");
		BINK_GET_TRACK_ID = GetProcAddress(H_BINKW32, "_BinkGetTrackID@8");

		BINK_SET_PAN = GetProcAddress(H_BINKW32, "_BinkSetPan@12");

		BINK_START_ASYNC_THREAD = GetProcAddress(H_BINKW32, "_BinkStartAsyncThread@8");
		BINK_SET_OS_FILE_CALLBACKS = GetProcAddress(H_BINKW32, "_BinkSetOSFileCallbacks@16");
		BINK_SET_WILL_LOOP = GetProcAddress(H_BINKW32, "_BinkSetWillLoop@8");
		BINK_DO_FRAME_ASYNC = GetProcAddress(H_BINKW32, "_BinkDoFrameAsync@12");
		BINK_DO_FRAME_ASYNC_WAIT = GetProcAddress(H_BINKW32, "_BinkDoFrameAsyncWait@8");

		BINK_OPEN_XAUDIO2 = GetProcAddress(H_BINKW32, "_BinkOpenXAudio2@4");

		BINK_GET_REALTIME = GetProcAddress(H_BINKW32, "_BinkGetRealtime@12");
	}
}

void bink::unload() {
	if (H_BINKW32 != nullptr) {
		FreeLibrary(H_BINKW32);
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkClose(BINK* bink)
{
	__asm jmp BINK_CLOSE;
}

extern "C" __declspec(dllexport) __declspec(naked) std::int32_t __stdcall BinkCopyToBufferRect(BINK* bink, void* dst, std::int32_t dst_pitch, std::uint32_t dst_height, std::uint32_t dst_x, std::uint32_t dst_y, std::uint32_t src_x, std::uint32_t src_y, std::uint32_t src_w, std::uint32_t src_h, std::uint32_t flags)
{
	__asm jmp BINK_COPY_TO_BUFFER_RECT;
}

extern "C" __declspec(dllexport) __declspec(naked) std::int32_t __stdcall BinkDoFrame(BINK* bink)
{
	__asm jmp BINK_DO_FRAME;
}

extern "C" __declspec(dllexport) __declspec(naked) std::int32_t __stdcall BinkGetRects(BINK* bink, std::uint32_t flags)
{
	__asm jmp BINK_GET_RECTS;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkNextFrame(BINK* bink)
{
	__asm jmp BINK_NEXT_FRAME;
}

extern "C" __declspec(dllexport) __declspec(naked) BINK* __stdcall BinkOpen(const char* path, std::uint32_t flags)
{
	__asm jmp BINK_OPEN;
}

extern "C" __declspec(dllexport) __declspec(naked) void* __stdcall BinkOpenDirectSound(std::uint32_t param)
{
	__asm jmp BINK_OPEN_DIRECTSOUND;
}

extern "C" __declspec(dllexport) __declspec(naked) std::int32_t __stdcall BinkPause(BINK* bink, std::int32_t pause)
{
	__asm jmp BINK_PAUSE;
}

extern "C" __declspec(dllexport) __declspec(naked) std::int32_t __stdcall BinkSetSoundSystem(void* open, std::uint32_t param)
{
	__asm jmp BINK_SET_SOUND_SYSTEM;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetSoundTrack(std::uint32_t total_tracks, std::uint32_t* tracks)
{
	__asm jmp BINK_SET_SOUND_TRACK;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetVolume(BINK* bink, std::uint32_t track_id, std::int32_t volume)
{
	__asm jmp BINK_SET_VOLUME;
}

extern "C" __declspec(dllexport) __declspec(naked) std::int32_t __stdcall BinkShouldSkip(BINK* bink)
{
	__asm jmp BINK_SHOULD_SKIP;
}

extern "C" __declspec(dllexport) __declspec(naked) std::int32_t __stdcall BinkWait(BINK* bink)
{
	__asm jmp BINK_WAIT;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGoto(BINK* bink, std::uint32_t frame, std::int32_t flags) {
	__asm jmp BINK_GO_TO;
}

extern "C" __declspec(dllexport) __declspec(naked) std::int32_t __stdcall BinkCopyToBuffer(BINK* bink, void* buffer, std::int32_t pitch, std::int32_t height, std::uint32_t x, std::uint32_t y, std::uint32_t flags) {
	__asm jmp BINK_COPY_TO_BUFFER;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetFrameBuffersInfo(BINK* bink, void* frame_buffers) {
	__asm jmp BINK_GET_FRAME_BUFFERS_INFO;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetSummary(BINK* bink, void* summary) {
	__asm jmp BINK_GET_SUMMARY;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkRegisterFrameBuffers(BINK* bink, void* frame_buffers) {
	__asm jmp BINK_REGISTER_FRAME_BUFFERS;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetMemory(void* allocator, void* deleter) {
	__asm jmp BINK_SET_MEMORY;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetSpeakerVolumes(BINK* bink, std::int32_t track_id, std::uint32_t* speaker_indices, std::int32_t* volumes, std::uint32_t total) {
	__asm jmp BINK_SET_SPEAKER_VOLUMES;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetTrackID(BINK* bink, std::uint32_t track_index) {
	__asm jmp BINK_GET_TRACK_ID;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetPan(BINK* bink, std::uint32_t track_id, std::int32_t pan) {
	__asm jmp BINK_SET_PAN;
}

extern "C" __declspec(dllexport) __declspec(naked) std::int32_t __stdcall BinkStartAsyncThread(std::int32_t thread_num, void const* param) {
	__asm jmp BINK_START_ASYNC_THREAD;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetOSFileCallbacks(void* open, void* read, void* seek, void* close) {
	__asm jmp BINK_SET_OS_FILE_CALLBACKS;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetWillLoop(BINK* bink, std::int32_t onoff) {
	__asm jmp BINK_SET_WILL_LOOP;
}

extern "C" __declspec(dllexport) __declspec(naked) std::int32_t __stdcall BinkDoFrameAsync(BINK* bink, std::int32_t thread1, std::int32_t thread2) {
	__asm jmp BINK_DO_FRAME_ASYNC;
}

extern "C" __declspec(dllexport) __declspec(naked) std::int32_t __stdcall BinkDoFrameAsyncWait(BINK* bink, std::int32_t microseconds) {
	__asm jmp BINK_DO_FRAME_ASYNC_WAIT;
}

extern "C" __declspec(dllexport) __declspec(naked) void* __stdcall BinkOpenXAudio2(std::int32_t unknown) {
	__asm jmp BINK_OPEN_XAUDIO2;
}

extern "C" __declspec(dllexport) __declspec(naked) std::uint32_t __stdcall BinkGetRealtime(std::uint32_t a, std::uint32_t b, std::uint32_t c)
{
    __asm jmp BINK_GET_REALTIME;
}
