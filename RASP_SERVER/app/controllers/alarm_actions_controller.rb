class AlarmActionsController < ApplicationController
  before_action :set_alarm_action, only: [:show, :edit, :update, :destroy]

  # GET /alarm_actions
  # GET /alarm_actions.json
  def index
    @alarm_actions = AlarmAction.all
  end

  # GET /alarm_actions/1
  # GET /alarm_actions/1.json
  def show
  end

  # GET /alarm_actions/new
  def new
    @alarm_action = AlarmAction.new


  end

  # GET /alarm_actions/1/edit
  def edit
  end

  # POST /alarm_actions
  # POST /alarm_actions.json
  def create
    @alarm_action = AlarmAction.new(alarm_action_params)

    if params[:alarm_action][:from]=='home'
      if @alarm_action.save
        redirect_to root_path
      else
        redirect_to root_path
      end
    else
      respond_to do |format|
        if @alarm_action.save
          format.html { redirect_to @alarm_action, notice: 'Alarm action was successfully created.' }
          format.json { render :show, status: :created, location: @alarm_action }
        else
          format.html { render :new }
          format.json { render json: @alarm_action.errors, status: :unprocessable_entity }
        end
      end
    end

  end

  # PATCH/PUT /alarm_actions/1
  # PATCH/PUT /alarm_actions/1.json
  def update
    respond_to do |format|
      if @alarm_action.update(alarm_action_params)
        format.html { redirect_to @alarm_action, notice: 'Alarm action was successfully updated.' }
        format.json { render :show, status: :ok, location: @alarm_action }
      else
        format.html { render :edit }
        format.json { render json: @alarm_action.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /alarm_actions/1
  # DELETE /alarm_actions/1.json
  def destroy
    @alarm_action.destroy
    respond_to do |format|
      format.html { redirect_to alarm_actions_url, notice: 'Alarm action was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_alarm_action
      @alarm_action = AlarmAction.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def alarm_action_params
      params.require(:alarm_action).permit(:disable_alarm)
    end
end
