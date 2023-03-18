#include <alsa/asoundlib.h>
#include<stdio.h>
#include<stdlib.h>

int main() {
    int err;
    snd_mixer_t *mixer;
    snd_mixer_selem_id_t *sid;
    snd_mixer_elem_t* elem;
    long minv, maxv, volume;

    // Open a mixer
    err = snd_mixer_open(&mixer, 0);
    if (err < 0) {
        printf("Error opening mixer: %s\n", snd_strerror(err));
        return 1;
    }

    // Attach to the default sound card
    err = snd_mixer_attach(mixer, "default");
    if (err < 0) {
        printf("Error attaching mixer to sound card: %s\n", snd_strerror(err));
        snd_mixer_close(mixer);
        return 1;
    }

    // Register the mixer elements
    err = snd_mixer_selem_register(mixer, NULL, NULL);
    if (err < 0) {
        printf("Error registering mixer elements: %s\n", snd_strerror(err));
        snd_mixer_close(mixer);
        return 1;
    }

    // Load the mixer elements
    err = snd_mixer_load(mixer);
    if (err < 0) {
        printf("Error loading mixer elements: %s\n", snd_strerror(err));
        snd_mixer_close(mixer);
        return 1;
    }

    // Get the mixer element for the master volume control
    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, "Master");
    elem = snd_mixer_find_selem(mixer, sid);
    if (!elem) {
        printf("Unable to find Master volume control\n");
        snd_mixer_close(mixer);
        return 1;
    }

    // Get the volume range and current volume
    snd_mixer_selem_get_playback_volume_range(elem, &minv, &maxv);
    snd_mixer_selem_get_playback_volume(elem, 0, &volume);

    // Set the volume to 50%
    snd_mixer_selem_set_playback_volume_all(elem, volume / 2);

    // Close the mixer
    snd_mixer_close(mixer);

    return 0;
}

