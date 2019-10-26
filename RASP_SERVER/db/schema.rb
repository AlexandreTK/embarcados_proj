# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 2019_10_26_220801) do

  # These are extensions that must be enabled in order to support this database
  enable_extension "plpgsql"

  create_table "actions", force: :cascade do |t|
    t.boolean "disable_alarm"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  create_table "alarm_actions", force: :cascade do |t|
    t.boolean "disable_alarm"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  create_table "alarms", force: :cascade do |t|
    t.integer "hour"
    t.integer "min"
    t.boolean "repeat_monday"
    t.boolean "repeat_tuesday"
    t.boolean "repeat_wednesday"
    t.boolean "repeat_thursday"
    t.boolean "repeat_friday"
    t.boolean "repeat_saturday"
    t.boolean "repeat_sunday"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  create_table "settings", force: :cascade do |t|
    t.string "operation_type"
    t.integer "minutes_to_next_action"
    t.integer "timezone_hours_diff"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
    t.string "mp3_file_name"
    t.string "mp3_content_type"
    t.bigint "mp3_file_size"
    t.datetime "mp3_updated_at"
  end

end
