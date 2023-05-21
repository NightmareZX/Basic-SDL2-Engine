#include "AudioManager.h"
#include "Logger.h"
#include "IniFile.h"

AudioManager* AudioManager::mInstance = nullptr;


AudioManager::AudioManager()
{
	//Mix_Init(MIX_INIT_MP3);
	Mix_Init(MIX_INIT_OGG);//use this instead of mp3
	constexpr Sint32 FREQUENCY = 44100;
	constexpr Uint16 FORMAT = AUDIO_S16SYS;
	//1 is mono, 2 is stereo
	constexpr Sint32 CHANNEL_TYPE = 2;
	constexpr Sint32 CHUNK_SIZE = 1024;
	Mix_OpenAudio(FREQUENCY ,FORMAT , CHANNEL_TYPE,CHUNK_SIZE);
	
	Mix_AllocateChannels(16);
	InitialiseChannels();

	mFadeInNextMusic = false;
	mIsMusicFadingOut = false;
	mNextMusicID = "";
	mAudioManagerMutex = SDL_CreateMutex();
}

void AudioManager::RegisterSound(String filename, Mix_MusicType type, String path)
{
	Mix_Chunk* soundfile = nullptr;
	String fileWithPath = "";

	switch (type)
	{
	case MUS_WAV:
		fileWithPath = path + filename + ".wav";
		soundfile = Mix_LoadWAV(fileWithPath.c_str());
		break;
	default:
		return;
		break;
	}

	if (soundfile == nullptr)
	{
		Logger::GetInstance()->Log("AudioManager.cpp: Error: Sound file at '" + fileWithPath + "' couldn't load. Reason: " + SDL_GetError());
		return;
	}
	
	mSoundMap.emplace(filename,soundfile);
}
void AudioManager::RegisterMusic(String filename, Mix_MusicType type, String path)
{
	Mix_Music* musicfile = nullptr;
	String fileWithPath = "";

	switch (type)
	{
	case MUS_OGG:
		fileWithPath = path + filename + ".ogg";
		musicfile = Mix_LoadMUS(fileWithPath.c_str());
		break;
	default:
		return;
		break;
	}

	if (musicfile == nullptr)
	{
		Logger::GetInstance()->Log("AudioManager.cpp: Error: Music file at '" + fileWithPath + "' couldn't load. Reason: " + SDL_GetError());
		return;
	}
	IniFile musicPropsFile;
	bool fileOpened = musicPropsFile.LoadFile(filename + ".ini");
	if (fileOpened)
	{
		double introEnd = musicPropsFile.GetDoubleValue("MusicProperties", "intro_end");
		if (introEnd > 0.0)
		{
			mIntroSectionMusicMap.emplace(filename, introEnd);
		}
	}

	mMusicMap.emplace(filename,musicfile);
}

void AudioManager::PlaySoundEffect(String soundID, GameAudioChannels channel, bool interupt, Sint32 loopCount)
{
	if (channel == CHAN_MUSIC)
	{
		Logger::GetInstance()->Log("AudioManager.cpp: Warning: Sound file with ID: '" + soundID + "' tried to play in the MUSIC channel! Not playing...");
		return;
	}
	if (!ValidateSound(soundID))
	{
		Logger::GetInstance()->Log("AudioManager.cpp: Error: Sound file with ID: '" + soundID + "' isn't registered!");
		return;
	}
	
	Sint32 availableChannel = Mix_GroupAvailable(channel);
	if (availableChannel != -1)
	{
		Mix_PlayChannel(availableChannel, mSoundMap[soundID], loopCount);
	}

	
}

void AudioManager::PlayMusic(String musicID, bool fadeOutCurrent, bool fadeInNext, Sint32 msToFadeOut, Sint32 msToFadeIn)
{
	if (!ValidateMusic(musicID))
	{
		Logger::GetInstance()->Log("AudioManager.cpp: Error: Music file with ID: '" + musicID + "' isn't registered!");
		return;
	}
	if (mIsMusicFadingOut)
	{
		//implement
		return;
	}

	if (fadeOutCurrent && (Mix_PlayingMusic() == 1))
	{
		mNextMusicID = musicID;
		mIsMusicFadingOut = true;
		mFadeInNextMusic = fadeInNext;
		mFadeInNextMusic_ms = msToFadeIn;
		Mix_FadeOutMusic(msToFadeOut);
		Mix_HookMusicFinished(&AudioManager::MusicStoppedHandle);
	}
	else if (fadeInNext)
	{
		if (IsMusicIntroduced(musicID))
		{
			Mix_HaltMusic();
			Mix_RewindMusic();
			Mix_FadeInMusic(mMusicMap[musicID], 0, msToFadeIn);
			Mix_HookMusicFinished(&AudioManager::MusicStoppedHandleIntro);
		}
		else
		{
			Mix_HaltMusic();
			Mix_RewindMusic();
			Mix_FadeInMusic(mMusicMap[musicID], -1, msToFadeIn);
		}
	}
	else
	{
		if (IsMusicIntroduced(musicID))
		{
			Mix_HaltMusic();
			Mix_RewindMusic();
			Mix_PlayMusic(mMusicMap[musicID], 0);
			Mix_HookMusicFinished(&AudioManager::MusicStoppedHandleIntro);
		}
		else
		{
			Mix_HaltMusic();
			Mix_RewindMusic();
			Mix_PlayMusic(mMusicMap[musicID], -1);
		}
	}
	mCurrentMusicID = musicID;
}

void AudioManager::MusicStoppedHandle()
{
	SDL_LockMutex(AudioManager::GetInstance()->mAudioManagerMutex);
	bool* fadeOut = &AudioManager::GetInstance()->mIsMusicFadingOut;
	bool* fadeIn = &AudioManager::GetInstance()->mFadeInNextMusic;
	String* nextMusicID = &AudioManager::GetInstance()->mNextMusicID;
	Uint32* ms_toFadein = &AudioManager::GetInstance()->mFadeInNextMusic_ms;
	Mix_Music* nextMusic = AudioManager::GetInstance()->mMusicMap[*nextMusicID];
	Mix_HaltMusic();//just in case
	Mix_HookMusicFinished(nullptr);
	if (*fadeOut) *fadeOut = false;
	
	if (*fadeIn)
	{
		if (AudioManager::GetInstance()->IsMusicIntroduced(*nextMusicID))
		{
			Mix_FadeInMusic(nextMusic, 0, static_cast<Sint32>(*ms_toFadein));
			Mix_HookMusicFinished(&AudioManager::MusicStoppedHandleIntro);
		}
		else
			Mix_FadeInMusic(nextMusic, -1, static_cast<Sint32>(*ms_toFadein));

		*fadeIn = false;
	}
	else
	{
		if (AudioManager::GetInstance()->IsMusicIntroduced(*nextMusicID))
		{
			Mix_PlayMusic(nextMusic, 0);
			Mix_HookMusicFinished(&AudioManager::MusicStoppedHandleIntro);
		}
		else
			Mix_PlayMusic(nextMusic, -1);
	}
	SDL_UnlockMutex(AudioManager::GetInstance()->mAudioManagerMutex);
}
void AudioManager::MusicStoppedHandleIntro()
{
	SDL_LockMutex(AudioManager::GetInstance()->mAudioManagerMutex);
	MusicIntroMap* introMap = &AudioManager::GetInstance()->mIntroSectionMusicMap;
	MusicMap* musicMap = &AudioManager::GetInstance()->mMusicMap;
	String* currMusic = &AudioManager::GetInstance()->mCurrentMusicID;
	Mix_HaltMusic();//just in case
	Mix_HookMusicFinished(&AudioManager::MusicStoppedHandleIntro);
	Mix_PlayMusic((*musicMap)[*currMusic], 0);
	Mix_SetMusicPosition((*introMap)[*currMusic]);
	SDL_UnlockMutex(AudioManager::GetInstance()->mAudioManagerMutex);
}

void AudioManager::Dispose()
{
	//halt every channel
	Mix_HaltChannel(-1);
	Mix_HaltMusic();
	//for (SoundMap::iterator it; it != mSoundMap.end(); it++)
	//{
	//	Mix_FreeChunk(it->second);
	//}
	//for (MusicMap::iterator it; it != mMusicMap.end(); it++)
	//{
	//	Mix_FreeMusic(it->second);
	//}
	Mix_CloseAudio();
	delete this;
}

bool AudioManager::ValidateMusic(String id)
{
	if (mMusicMap.find(id) != mMusicMap.end())
		return true;
	return false;
}

bool AudioManager::ValidateSound(String id)
{
	if (mSoundMap.find(id) != mSoundMap.end())
		return true;
	return false;
}
bool AudioManager::IsMusicIntroduced(String id)
{
	if (mIntroSectionMusicMap.find(id) != mIntroSectionMusicMap.end())
		return true;
	return false;
}
void AudioManager::SetChannelVolume(GameAudioChannels channel, Uint32 volumePercent)
{
	//if the volume percent is higher than 100, then just set it to 100, otherwise dont change it
	volumePercent = (volumePercent > 100) ? 100: volumePercent;

	Sint32 volumeAdjusted = static_cast<Sint32>(static_cast<float>(MIX_MAX_VOLUME) * (static_cast<float>(volumePercent) / 100.0f));
	switch (channel)
	{
	case CHAN_MUSIC:
		Mix_Volume(0,volumeAdjusted);
		Mix_VolumeMusic(volumeAdjusted);
		break;
	case CHAN_PLAYER:
		Mix_Volume(1, volumeAdjusted);
		break;
	case CHAN_WEAPONS:
		Mix_Volume(5, volumeAdjusted);
		Mix_Volume(6, volumeAdjusted);
		break;
	case CHAN_ENVIRONMENT:
		Mix_Volume(2, volumeAdjusted);
		break;
	case CHAN_UI:
		Mix_Volume(3, volumeAdjusted);
		Mix_Volume(4, volumeAdjusted);
		break;
	case CHAN_ANY:
		Mix_Volume(7, volumeAdjusted);
		Mix_Volume(8, volumeAdjusted);
		Mix_Volume(9, volumeAdjusted);
		Mix_Volume(10, volumeAdjusted);
		Mix_Volume(11, volumeAdjusted);
		Mix_Volume(12, volumeAdjusted);
		Mix_Volume(13, volumeAdjusted);
		Mix_Volume(14, volumeAdjusted);
		Mix_Volume(15, volumeAdjusted);
		break;
	default:
		break;
	}
}
void AudioManager::InitialiseChannels()
{
	//Grouping channels so that they can be used for specific checks and sounds
	Mix_GroupChannel(0, CHAN_MUSIC);
	Mix_GroupChannel(1, CHAN_PLAYER);
	Mix_GroupChannel(2, CHAN_ENVIRONMENT);
	Mix_GroupChannel(3, CHAN_UI);//may revise
	Mix_GroupChannel(4, CHAN_UI);//
	Mix_GroupChannel(5, CHAN_WEAPONS);
	Mix_GroupChannel(6, CHAN_WEAPONS);
	Mix_GroupChannel(7, CHAN_ANY);
	Mix_GroupChannel(8, CHAN_ANY);
	Mix_GroupChannel(9, CHAN_ANY);
	Mix_GroupChannel(10, CHAN_ANY);
	Mix_GroupChannel(11, CHAN_ANY);
	Mix_GroupChannel(12, CHAN_ANY);
	Mix_GroupChannel(13, CHAN_ANY);
	Mix_GroupChannel(14, CHAN_ANY);
	Mix_GroupChannel(15, CHAN_ANY);
}