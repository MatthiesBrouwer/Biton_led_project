int StationStorage::getActiveStations(){
  return stations_active;
}

bool StationStorage::addLedstripStation(
                           uint8_t station_address[6], 
                           char* station_name){
    if(stations_active >= MAX_STATIONS){
      Serial.println("STATIONS FULL!!");
      return false;
    }

    //LedstripStation new_station(station_name, station_address);
    ledstrip_stations[stations_active] = new LedstripStation(station_name, station_address);
    stations_active++;
    Serial.print("NEW STATION ADDED. NOW ACTIVE: ");Serial.println(stations_active);                    
    return true;
  }


LedstripStation* StationStorage::getStation(uint8_t station_address[6]){
  if(stations_active < 1){
    return NULL;
  }
  for(int i = 0; i < stations_active; i++){
    if(ledstrip_stations[i]->station_address == station_address){
      return ledstrip_stations[i];
    }
  }

  return NULL;
}


LedstripStation* StationStorage::getStation(char* station_name){
  if(stations_active < 1){
    return NULL;
  }
  for(int i = 0; i < stations_active; i++){
    if(ledstrip_stations[i]->station_name == station_name){
      return ledstrip_stations[i];
    }
  }

  return NULL;
}

LedstripStation* StationStorage::getStation(int index){
  if(stations_active < 1){
    return NULL;
  }

  return ledstrip_stations[index];
  
}
