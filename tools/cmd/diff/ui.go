// License: GPLv3 Copyright: 2023, Kovid Goyal, <kovid at kovidgoyal.net>

package diff

import (
	"fmt"
	"kitty/tools/tui/loop"
)

var _ = fmt.Print

type ResultType int

const (
	COLLECTION ResultType = iota
	DIFF
	HIGHLIGHT
)

type Reference struct {
}

type AsyncResult struct {
	err        error
	rtype      ResultType
	collection *Collection
	diff_map   map[string]*Patch
}

type Handler struct {
	async_results                                 chan AsyncResult
	left, right                                   string
	collection                                    *Collection
	diff_map                                      map[string]*Patch
	lp                                            *loop.Loop
	current_context_count, original_context_count int
	added_count, removed_count                    int
}

func (self *Handler) calculate_statistics() {
	self.added_count, self.removed_count = self.collection.added_count, self.collection.removed_count
	for _, patch := range self.diff_map {
		self.added_count += patch.added_count
		self.removed_count += patch.removed_count
	}
}

func (self *Handler) initialize() {
	self.current_context_count = opts.Context
	if self.current_context_count < 0 {
		self.current_context_count = int(conf.Num_context_lines)
	}
	self.original_context_count = self.current_context_count
	self.lp.SetDefaultColor(loop.FOREGROUND, conf.Foreground)
	self.lp.SetDefaultColor(loop.CURSOR, conf.Foreground)
	self.lp.SetDefaultColor(loop.BACKGROUND, conf.Background)
	self.lp.SetDefaultColor(loop.SELECTION_BG, conf.Select_bg)
	if !conf.Select_fg.IsNull {
		self.lp.SetDefaultColor(loop.SELECTION_FG, conf.Select_fg.Color)
	}
	self.async_results = make(chan AsyncResult, 32)
	go func() {
		r := AsyncResult{}
		r.collection, r.err = create_collection(self.left, self.right)
		self.async_results <- r
		self.lp.WakeupMainThread()
	}()
}

func (self *Handler) generate_diff() {
	self.diff_map = nil
	jobs := make([]diff_job, 0, 32)
	self.collection.Apply(func(path, typ, changed_path string) error {
		if typ == "diff" {
			if is_path_text(path) && is_path_text(changed_path) {
				jobs = append(jobs, diff_job{path, changed_path})
			}
		}
		return nil
	})
	go func() {
		r := AsyncResult{rtype: DIFF}
		r.diff_map, r.err = diff(jobs, self.current_context_count)
		self.async_results <- r
		self.lp.WakeupMainThread()
	}()
}

func (self *Handler) on_wakeup() error {
	var r AsyncResult
	for {
		select {
		case r = <-self.async_results:
			if r.err != nil {
				return r.err
			}
			r.err = self.handle_async_result(r)
			if r.err != nil {
				return r.err
			}
		default:
			return nil
		}
	}
}

func (self *Handler) handle_async_result(r AsyncResult) error {
	switch r.rtype {
	case COLLECTION:
		self.collection = r.collection
		self.generate_diff()
	case DIFF:
		self.diff_map = r.diff_map
		self.calculate_statistics()
		self.render_diff()
		self.scroll_pos = 0
		if self.restore_position != nil {
			self.set_current_position(self.restore_position)
			self.restore_position = nil
		}
		self.draw_screen()
	case HIGHLIGHT:
	}
	return nil
}
