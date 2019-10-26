class CreateSettings < ActiveRecord::Migration[5.2]
  def change
    create_table :settings do |t|
      t.string :operation_type
      t.integer :minutes_to_next_action
      t.integer :timezone_hours_diff

      t.timestamps
    end
  end
end
