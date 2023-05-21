#pragma once
#include "SDL.h"
#include "SDL_mixer.h"
#include "STL_Components.h"


//
enum GameAudioChannels
{
	//Channel for music only
	CHAN_MUSIC = 1,
	//used by the player
	CHAN_PLAYER = 2,
	//used by the player's weapons
	CHAN_WEAPONS = 3,
	//Used by the map 
	CHAN_ENVIRONMENT = 4,
	//Used by menus and such
	CHAN_UI = 5,
	//Any is used by enemies and other stuff that might need it
	CHAN_ANY = 6
};

//Used for sound effects mainly since they are short and can be stored in memory
using SoundMap = map<String, Mix_Chunk*>;
//Used for music as they are longer, so they have to be read from the drive instead of being in memory
using MusicMap = map<String, Mix_Music*>;
//this map holds the IDs of music which have an intro section, it also stores where this intro section ends
using MusicIntroMap = map<String, double>;

class AudioManager
{
private:
	static AudioManager* mInstance;

	SoundMap mSoundMap;
	MusicMap mMusicMap;

	//Creating a mutex because some of the functions arent thread safe
	SDL_mutex* mAudioManagerMutex;

	bool mFadeInNextMusic;
	Uint32 mFadeInNextMusic_ms;

	bool mIsMusicFadingOut;

	String mNextMusicID;
	//The id of the music currently playing
	String mCurrentMusicID;

	MusicIntroMap mIntroSectionMusicMap;
	AudioManager();
	bool ValidateMusic(String id);
	bool ValidateSound(String id);
	//Return if the music has an intro section that needs to play only once, but not every other loop after it
	bool IsMusicIntroduced(String id);
	void PlayMusicWithIntro();
	void InitialiseChannels();
public:
	
	inline static AudioManager* GetInstance() { if (mInstance == nullptr) mInstance = new AudioManager; return mInstance; }
	void RegisterSound(String filename, Mix_MusicType type, String path = "");
	void RegisterMusic(String filename, Mix_MusicType type, String path = "");
	void PlaySoundEffect(String soundID, GameAudioChannels channel, bool interupt = true, Sint32 loopCount = 0);
	/// <summary>
	/// The function that is used to play an already registered piece of music
	/// </summary>
	/// <param name="musicID">: The id of the registered music that should play</param>
	/// <param name="fadeOutCurrent">: Wether the currently playing music should be faded out (does nothing if no music is playing)</param>
	/// <param name="fadeInNext">: Wether the music that is being passed in should be faded in after the current one ends</param>
	/// <param name="msToFadeOut">: The milliseconds that it should take to fade out the currently playing music</param>
	/// <param name="msToFadeIn">: The milliseconds that it should take to fade in the next music</param>
	void PlayMusic(String musicID, bool fadeOutCurrent = false, bool fadeInNext = false, Sint32 msToFadeOut = 0, Sint32 msToFadeIn = 0);
	/// <summary>
	/// Sets the volume of a specific group of channels
	/// </summary>
	/// <param name="channel">: the name of the group</param>
	/// <param name="volumePercent">: the volume percent ranging from 0 - 100</param>
	void SetChannelVolume(GameAudioChannels channel, Uint32 volumePercent);
	static void MusicStoppedHandle();
	static void MusicStoppedHandleIntro();
	void Dispose();
};