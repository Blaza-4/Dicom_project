import gi
gi.require_version('GeoClue', '2.0')
from gi.repository import GeoClue

def on_location_changed(agent, location):
    print("Location changed:")
    print("Latitude:", location.get_latitude())
    print("Longitude:", location.get_longitude())

# Create a GeoClue client
client = GeoClue.Client.new_sync()
agent = client.get_default_agent_sync()

# Connect to the location changed signal
agent.connect("notify::location", on_location_changed)

# Run the main loop
import time
while True:
    time.sleep(1)