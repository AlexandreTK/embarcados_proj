# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rails db:seed command (or created alongside the database with db:setup).
#
# Examples:
#
#   movies = Movie.create([{ name: 'Star Wars' }, { name: 'Lord of the Rings' }])
#   Character.create(name: 'Luke', movie: movies.first)

mp3_path = "#{Rails.root}/Birds Song.mp3"
mp3_file = File.new(mp3_path)

Setting.create(operation_type: 'Open Window and Play Song', minutes_to_next_action: 10, timezone_hours_diff: -3, mp3: mp3_file)
