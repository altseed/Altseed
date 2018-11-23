#include "asd.MediaPlayerAVF.h"

#ifdef __APPLE__

#include "../../asd.Graphics.h"
#include "../../asd.Graphics_Imp.h"


#import <AVFoundation/AVFoundation.h>
#import <Cocoa/Cocoa.h>

@interface MediaPlayerAVFPlayer : NSObject

@property bool isLoopingMode;
@property AVPlayer* _player;
@property AVPlayerItem* _playerItem;

@end

@implementation MediaPlayerAVFPlayer

-(void)observe {
    //self._player.actionAtItemEnd = AVPlayerActionAtItemEndNone;
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(playerItemDidReachEnd:)
                                                 name:AVPlayerItemDidPlayToEndTimeNotification
                                               object:self._playerItem];
}

-(void)remove {
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                 name:AVPlayerItemDidPlayToEndTimeNotification
                                               object:self._playerItem];
}

- (void)playerItemDidReachEnd:(NSNotification *)notification {
    AVPlayerItem* p = [notification object];
    if(self.isLoopingMode)
    {
        [p seekToTime:kCMTimeZero];
    }
}

@end

namespace asd
{
    
class MediaPlayerAVF_Impl
{
public:
        
    AVPlayer* _player = nullptr;
    AVPlayerItem* _playerItem = nullptr;
    AVPlayerItemVideoOutput* _playerItemVideoOutput = nullptr;
    AVURLAsset* _asset = nullptr;
    MediaPlayerAVFPlayer* _playerWrapper = nullptr;
    
    int movieWidth = 0;
    int movieHeight = 0;
    bool isLoaded = false;
    bool isPlaying = false;
    float frameRate = 0;
    bool isLoopingMode = false;
    
    MediaPlayerAVF_Impl()
    {
    }
        
    virtual ~MediaPlayerAVF_Impl()
    {
        reset();
    }
        
    void reset()
    {
        if(isPlaying)
        {
            [_player pause];
            isPlaying = false;
        }
        
        if(_playerWrapper != nullptr)
        {
            [_playerWrapper remove];
            [_playerWrapper release];
            _playerWrapper = nullptr;
        }

        if(_asset != nullptr)
        {
            [_asset release];
            _asset = nullptr;
        }
        
        if(_playerItem!= nullptr)
        {
            [_playerItem release];
            _playerItem = nullptr;
        }
        
        if(_player != nullptr)
        {
            [_player release];
            _player = nullptr;
        }
        
        if(_playerItemVideoOutput != nullptr)
        {
            [_playerItemVideoOutput release];
            _playerItemVideoOutput = nullptr;
        }
        
        _player = nullptr;
        _playerItem = nullptr;
            
        isLoaded = false;
    }
        
    bool initialize(const char* path)
    {
        reset();
            
        NSString* urlString = [NSString stringWithCString:path encoding:NSUTF8StringEncoding];
        NSURL* url = [NSURL fileURLWithPath:urlString];
            
        NSDictionary* options = @{ AVURLAssetPreferPreciseDurationAndTimingKey : @YES };
            
        _asset = [[AVURLAsset alloc] initWithURL:url options:options];
            
        __block bool finished = false;
        __block bool hasError = false;
            
        NSArray *keys = @[@"preferredTransform"];
            
        [_asset loadValuesAsynchronouslyForKeys:keys completionHandler:^() {
                
            NSError *error = nil;
            AVKeyValueStatus tracksStatus = [_asset statusOfValueForKey:@"preferredTransform" error:&error];
            switch (tracksStatus) {
                case AVKeyValueStatusLoaded:
                    break;
                case AVKeyValueStatusFailed:
                    NSLog(@"The asset's tracks were not loaded:\n%@", [error localizedDescription]);
                    hasError = true;
                    break;
                case AVKeyValueStatusCancelled:
                    break;
            }
            finished = true;
        }];
            
        while(!finished)
        {
            usleep(100);
        }
            
        if(hasError)
        {
            return false;
            }
            
        CGSize naturalSize = [[[_asset tracksWithMediaType:AVMediaTypeVideo] objectAtIndex:0] naturalSize];
        movieWidth = naturalSize.width;
        movieHeight = naturalSize.height;
        frameRate = [[[_asset tracksWithMediaType:AVMediaTypeVideo] objectAtIndex:0] nominalFrameRate];
        
        NSDictionary *videoOutputOptions = [NSDictionary dictionaryWithObjectsAndKeys:
                                                [NSNumber numberWithInt:kCVPixelFormatType_32BGRA], kCVPixelBufferPixelFormatTypeKey,
                                                [NSDictionary dictionary], kCVPixelBufferIOSurfacePropertiesKey,
                                                nil];
            
        _playerItemVideoOutput = [[AVPlayerItemVideoOutput alloc] initWithPixelBufferAttributes:videoOutputOptions];
        _playerItem = [AVPlayerItem playerItemWithAsset:_asset];
            
        _playerItemVideoOutput.suppressesPlayerRendering = true;
        [_playerItem addOutput: _playerItemVideoOutput];
        [_playerItemVideoOutput requestNotificationOfMediaDataChangeWithAdvanceInterval:1];
            
        _player = [AVPlayer playerWithPlayerItem:_playerItem];
        [_player seekToTime:kCMTimeZero];
        
        [_player retain];
        [_playerItem retain];
        
        _player.actionAtItemEnd = AVPlayerActionAtItemEndNone;
        
        _playerWrapper = [MediaPlayerAVFPlayer alloc];
        _playerWrapper._player = _player;
        _playerWrapper._playerItem = _playerItem;
        [_playerWrapper observe];
        
        isLoaded = true;
            
        return true;
    }
        
        void play(bool isLoopingMode)
        {
            if(!isLoaded) return;
            if(isPlaying) return;
            this->isLoopingMode = isLoopingMode;
            isPlaying = true;
            _playerWrapper.isLoopingMode = this->isLoopingMode;
            [_player play];
        }
    
        int getCurrentFrame()
        {
            CMTime currentTime = _playerItem.currentTime;
            float time = ((float)currentTime.value / (float)currentTime.timescale);
            return time * frameRate;
        }
        
        void getFrame(uint8_t* data)
        {
            if(_playerItem.status != AVPlayerStatusReadyToPlay)
            {
                return;
            }
            
            CMTime currentTime = _playerItem.currentTime;
            
            CVPixelBufferRef pixelBuffer = [_playerItemVideoOutput copyPixelBufferForItemTime:currentTime itemTimeForDisplay: nullptr];
            
            if(pixelBuffer == nullptr)
            {
                return;
            }
            
            CVPixelBufferLockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);
            int width = CVPixelBufferGetWidth(pixelBuffer);
            int height = CVPixelBufferGetHeight(pixelBuffer);
            uint8_t* baseAddress = (uint8_t*)CVPixelBufferGetBaseAddress(pixelBuffer);
            int bytesPerRow = CVPixelBufferGetBytesPerRow(pixelBuffer);
            
            for(int y = 0; y < height; y++)
            {
                for(int x = 0; x < width; x++)
                {
                    data[(x + y * width) * 4 + 0] = baseAddress[x * 4 + 2 + y * bytesPerRow];
                    data[(x + y * width) * 4 + 1] = baseAddress[x * 4 + 1 + y * bytesPerRow];
                    data[(x + y * width) * 4 + 2] = baseAddress[x * 4 + 0 + y * bytesPerRow];
                    data[(x + y * width) * 4 + 3] = baseAddress[x * 4 + 3 + y * bytesPerRow];
                }
            }
            
            CVPixelBufferUnlockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);
        }
    };
    
	MediaPlayerAVF::MediaPlayerAVF(Graphics* graphics)
		: graphics(graphics)
	{
		SafeAddRef(graphics);
        impl = std::make_shared<MediaPlayerAVF_Impl>();
	}

	MediaPlayerAVF::~MediaPlayerAVF()
	{
		SafeRelease(graphics);
	}

	bool MediaPlayerAVF::Play(bool isLoopingMode)
	{
        impl->play(isLoopingMode);
		return true;
	}

	void MediaPlayerAVF::Write(void* dst)
	{
        impl->getFrame((uint8_t*)dst);
	}

	bool MediaPlayerAVF::SetSourceFromPath(const char16_t* path)
	{
        std::vector<int8_t> dst;
        Utf16ToUtf8(dst, (const int16_t*)path);
        return impl->initialize((const char*)dst.data());
	}

	int32_t MediaPlayerAVF::GetWidth() const
	{
        return impl->movieWidth;
	}

	int32_t MediaPlayerAVF::GetHeight() const
	{
        return impl->movieHeight;
	}

	int32_t MediaPlayerAVF::GetCurrentFrame() const
	{
        return impl->getCurrentFrame();
	}

	bool MediaPlayerAVF::Load(const achar* path)
	{
		return SetSourceFromPath(path);
	}

	bool MediaPlayerAVF::WriteToTexture2D(Texture2D* target)
	{
		if (target == nullptr) return false;
		if (target->GetSize() != GetSize()) return false;
		if (!(target->GetFormat() == TextureFormat::R8G8B8A8_UNORM || target->GetFormat() == TextureFormat::R8G8B8A8_UNORM_SRGB)) return false;

		TextureLockInfomation l;

		if (target->Lock(&l))
		{
			Write(l.GetPixels());

			target->Unlock();
			return true;
		}

		return false;
	}
}

#endif
