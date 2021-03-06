#include <iostream>
#include <exception>
#include <assert.h>

#include <mangle/stream/servers/file_stream.hpp>
#include <mangle/sound/filters/openal_audiere.hpp>

#include <sound/sndmanager.hpp>

using namespace std;
using namespace Mangle::Stream;
using namespace Mangle::Sound;
using namespace OEngine::Sound;

const std::string sound = "../../mangle/sound/tests/cow.wav";

int main()
{
  SoundFactoryPtr oaf(new OpenAL_Audiere_Factory);
  SoundManagerPtr mg(new SoundManager(oaf));

  cout << "Playing " << sound << "\n";

  assert(mg->numSounds() == 0);

  /** Start the sound playing, and then let the pointer go out of
      scope. Lower-level players (like 'oaf' above) will immediately
      delete the sound. SoundManager OTOH will keep it until it's
      finished.
  */
  mg->play(sound);

  assert(mg->numSounds() == 1);

  // Loop while there are still sounds to manage
  while(mg->numSounds() != 0)
    {
      assert(mg->numSounds() == 1);
      usleep(10000);
      if(mg->needsUpdate)
        mg->update();
    }

  SoundPtr snd = mg->play(sound);
  cout << "Replaying\n";
  int i = 0;
  while(mg->numSounds() != 0)
    {
      assert(mg->numSounds() == 1);
      usleep(10000);
      if(mg->needsUpdate)
        mg->update();

      if(i++ == 70)
        {
          cout << "pause\n";
          snd->pause();
        }
      if(i == 130)
        {
          cout << "restart\n";
          snd->play();
          // Let the sound go out of scope
          snd.reset();
        }
    }

  cout << "Done playing.\n";

  assert(mg->numSounds() == 0);

  return 0;
}
