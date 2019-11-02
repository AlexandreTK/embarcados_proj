import sys
from telegram.ext import Updater, MessageHandler, Filters

try:
    f= open(".token","r")
except:
    print("Add bot token to the '.token' file in the same directory as this script")
    sys.exit(0);
token = f.read().rstrip('\n')
updater = Updater(token=token)

dispatcher = updater.dispatcher

def voice_handler(bot, update):
    file = bot.getFile(update.message.voice.file_id)
    file.download('voice.ogg')
    print('updated song')


dispatcher.add_handler(MessageHandler(Filters.voice, voice_handler))

updater.start_polling()

