use Bitwise

defmodule Counter do
  use Agent

  def start_link(initial_value) do
    Agent.start_link(fn -> initial_value end, name: __MODULE__)
  end

  def value do
    Agent.get(__MODULE__, & &1)
  end

  def increment do
    id = Agent.get(__MODULE__, & &1)
    if id  == 1024 do
	Agent.update(__MODULE__, 0)
    end
  end

end

defmodule GlobalId do
  @moduledoc """
  GlobalId module contains an implementation of a guaranteed globally unique id system.     
  """

  @doc """
  Please implement the following function.
  64 bit non negative integer output   
  """
  @spec get_id() :: non_neg_integer
  def get_id() do
  	t = timestamp() <<< (63-41) 
	id = node_id() <<< (63-41-10)
	s = Counter.value()
	Counter.increment()
	t ||| id ||| s
  end

  #
  # You are given the following helper functions
  # Presume they are implemented - there is no need to implement them. 
  #

  @doc """
  Returns your node id as an integer.
  It will be greater than or equal to 0 and less than or equal to 1024.
  It is guaranteed to be globally unique. 
  """
  @spec node_id() :: non_neg_integer
  def node_id() do
	1023
  end

  @doc """
  Returns timestamp since the epoch in milliseconds. 
  """
  @spec timestamp() :: non_neg_integer
  def timestamp() do 
	1579248831791
  end
end

Counter.start_link(0)

IO.puts GlobalId.get_id()
IO.puts GlobalId.get_id()
IO.puts GlobalId.get_id()
