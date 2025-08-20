import requests
#Using api url to get puuid for matches
api_key = "RGAPI-b0646691-5a5d-44ea-a82f-4fde409c4b28"
api_url = "https://americas.api.riotgames.com/riot/account/v1/accounts/by-riot-id/jonfam/jon?api_key=RGAPI-210acbc8-b269-482a-b52d-a76c016bdc54"
response = requests.get(api_url)
player_info = response.json()
puuid = "Ggv8rqwlEYPuL8Z3_uSjBeLDCAweej2wuMxE9ugxVSLsnlOKyaw0Wp76bRuzGtvLwdEqpUGwZKPAPQ"
#print("Player info: ", player_info, "\n")

#plug puuid into riot api to get match urls
matchhistory_url = "https://americas.api.riotgames.com/lol/match/v5/matches/by-puuid/Ggv8rqwlEYPuL8Z3_uSjBeLDCAweej2wuMxE9ugxVSLsnlOKyaw0Wp76bRuzGtvLwdEqpUGwZKPAPQ/ids?start=0&count=20&api_key=RGAPI-210acbc8-b269-482a-b52d-a76c016bdc54"
response = requests.get(matchhistory_url)
match_ids = response.json()
#print("Recent 20 match ids: ", match_ids, "\n")

#plug in match url
match_url = "https://americas.api.riotgames.com/lol/match/v5/matches/NA1_4968261464?api_key=RGAPI-210acbc8-b269-482a-b52d-a76c016bdc54"
response = requests.get(match_url)
match_data = response.json()

#getting specific player
participants = match_data['metadata']['participants']
player_index = participants.index(puuid)
player_data = match_data['info']['participants'][player_index]
player_name = match_data['info']['participants'][player_index]['summonerName']

#now i can output whatever statistics i want
gamemode = match_data['info']['gameMode']
champ = player_data['championName']
kills = player_data['kills']
deaths = player_data['deaths']
assists = player_data['assists']
win = player_data['win']
print("Player: ", player_name)
print("Game Mode: ", gamemode)
print("Champion: ", champ)
print("Kills: ", kills)
print("Deaths: ", deaths)
print("Winner?: ", win)
print(match_data)

#function to get puuid, given a riot ID, tagline, and region


