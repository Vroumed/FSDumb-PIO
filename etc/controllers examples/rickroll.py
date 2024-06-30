import time
import pretty_midi
import websocket
import json

uri = "ws://192.168.1.22/ws"
ws = websocket.WebSocket()
ws.connect(uri)

def midi_to_frequency(note):
    """Convert MIDI note number to frequency."""
    return pretty_midi.note_number_to_hz(note)

def extract_midi_data(file_path):
    """Extract note data from MIDI file and return as list of tuples (frequency, start_time, end_time, duration, next_start_time)."""
    midi_data = pretty_midi.PrettyMIDI(file_path)
    note_data = []

    for instrument in midi_data.instruments:
        for i, note in enumerate(instrument.notes):
            frequency = midi_to_frequency(note.pitch)
            start_time = note.start
            end_time = note.end
            duration = end_time - start_time
            next_start_time = instrument.notes[i + 1].start if i + 1 < len(instrument.notes) else None
            note_data.append((frequency, start_time, end_time, duration, next_start_time))
    
    return note_data

def HornOn(freq):
    ws.send(json.dumps({"cmd": 2, "data": {"headX":0, "headY": freq / 1000 }}))
    ws.send(json.dumps({"cmd": 3, "data": {"horn":1, "frequency": freq }}))

def HornOff(freq):
    ws.send(json.dumps({"cmd": 3, "data": {"horn":0, "frequency": freq }}))

def main(file_path, playback_speed = 1.0):
    note_data = extract_midi_data(file_path)
    
    start_time = 0  # Temps initial

    for i, (frequency, start_time, end_time, duration, next_start_time) in enumerate(note_data):
        # Attendre jusqu'à la prochaine note
        if i > 0:
            time.sleep((start_time - previous_end_time) / playback_speed)

        # Allumer le buzzer pour la note
        HornOn(frequency)
        
        # Attendre la durée de la note
        time.sleep(duration / playback_speed)
        
        # Éteindre le buzzer après la note
        HornOff(frequency)
        
        # Calculer l'espace entre les notes
        if next_start_time is not None:
            space = next_start_time - end_time
            if space > 0:
                time.sleep(space / playback_speed)
        
        previous_end_time = end_time

if __name__ == "__main__":
    main('rickroll.mid')
