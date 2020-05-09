#import "AudioPlayerViewController.h"

@implementation AudioPlayerViewController


- (void)viewDidLoad {
    [super viewDidLoad];

	NSURL *url = [NSURL fileURLWithPath:[NSString stringWithFormat:@"%@/audiofile.mp3", [[NSBundle mainBundle] resourcePath]]];
	
	NSError *error;
	audioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
	audioPlayer.numberOfLoops = -1;
	
	if (audioPlayer == nil)
		NSLog([error description]);				
	else 
		[audioPlayer play];

}

- (void)dealloc {
	[audioPlayer release];
    [super dealloc];
}

@end
