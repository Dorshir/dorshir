#include <stdio.h>

enum ColorEnum {
    RED,
    GREEN,
    DEFAULT
};

static void setColor(enum ColorEnum color)
{
    static const char *pallete[] = {"\x1B[31m", "\x1B[32m", "\033[0m"};
    puts(pallete[color]);
}

int Shape_NumOfShapes = 0;

struct Scaleable {
    void (*scale)(struct Scaleable* self, double f);
};

struct Shape {

    int m_id;

    void (*draw)(const struct Shape* self);
    void (*draw_color)(const struct Shape* self, enum ColorEnum c);
    void (*scale)(const struct Shape* self, double f);
    double (*area)(const struct Shape* self);
};

void Shape_printInventory()
{
    printf("Shape::printInventory - %d\n", Shape_NumOfShapes);
}

void Shape_draw(const struct Shape* self)
{
    printf("Shape::draw() - %d\n", self->m_id);
}

void Shape_draw_color(const struct Shape* self, enum ColorEnum c)
{
    setColor(c);
    printf("Shape::draw(%d)\n", self->m_id);
    Shape_draw(self);
    setColor(DEFAULT);
}

void Shape_scale_empty(const struct Shape* self, double f)
{
    (void) self;
    (void) f;
}

double Shape_area_empty(const struct Shape* self)
{
    (void) self;
    return -1;
}

void Shape_init(struct Shape* self)
{
    self->m_id = ++Shape_NumOfShapes;
    self->draw = Shape_draw;
    self->draw_color = Shape_draw_color;
    self->scale = Shape_scale_empty;
    self->area = Shape_area_empty;
    printf("Shape::CTOR - %d\n", self->m_id);
}

void Shape_init_copy(struct Shape* self, const struct Shape* other)
{
    self->m_id = ++Shape_NumOfShapes;
    self->draw = other->draw;
    self->draw_color = other->draw_color;
    self->scale = other->scale;
    self->area = other->area;

    printf("Shape::CCTOR %d from - %d\n", self->m_id, other->m_id);
}

void Shape_destroy(const struct Shape* self)
{
    Shape_draw(self);
    printf("Shape::DTOR - %d\n", self->m_id);
    --Shape_NumOfShapes; 
}



double drawBig(struct Shape *shape)
{
    double a0 = shape->area(shape);
    shape->scale(shape, 2);
    double a1 = shape->area(shape);
    return a1 - a0;
}


struct Circle {

    struct Shape base;
    double m_radius;

};

void Circle_draw(const struct Shape* self)
{
    printf("Circle::draw()  - %d, r:%f\n", ((struct Circle *)self)->base.m_id, ((struct Circle *)self)->m_radius);
}

void Circle_scale(const struct Shape* self, double f)
{
    ((struct Circle *)self)->m_radius *= f;
}

double Circle_area(const struct Shape* self)
{
    return ((struct Circle *)self)->m_radius * ((struct Circle *)self)->m_radius * 3.1415;
}

double Circle_radius(const struct Circle* self)
{
    printf("Circle::draw()  - %d, r:%f\n", self->base.m_id, self->m_radius);
    return self->m_radius;
}

void Circle_init(struct Circle* self)
{
    Shape_init(&self->base);

    self->base.area = Circle_area;
    self->base.draw = Circle_draw;
    self->base.scale = Circle_scale;

    self->m_radius = 1;
    printf("Circle::CTOR() - %d, r:%f\n", self->base.m_id, self->m_radius); 
}

void Circle_init_radius(struct Circle* self, double r)
{
    Shape_init(&self->base);

    self->base.area = Circle_area;
    self->base.draw = Circle_draw;
    self->base.scale = Circle_scale;

    self->m_radius = r;
    printf("Circle::CTOR(double) - %d, r:%f\n", self->base.m_id, self->m_radius); 
}

void Circle_init_copy(struct Circle* self, const struct Circle* other)
{
    Shape_init_copy(&self->base, &other->base);

    self->base.area = Circle_area;
    self->base.draw = Circle_draw;
    self->base.scale = Circle_scale;

    self->m_radius = other->m_radius;
    printf("Circle::CCTOR - %d, r:%f\n", self->base.m_id, self->m_radius);
}

void Circle_destroy(struct Circle* self)
{
    printf("Circle::DTOR - %d, r:%f\n", self->base.m_id, self->m_radius);
}

struct Circle unit;


void Global_circle_draw(struct Circle* c)
{
    Circle_draw((const struct Shape *)&unit);
    Circle_scale((const struct Shape *)&unit, 3);

    Circle_draw((const struct Shape *)c);
}


int main()
{
    struct Circle c;

    Circle_init(&c);

    struct Circle copy;

    Circle_init_copy(&copy, &c);

    Circle_init_radius(&unit, 1);

    Global_circle_draw(&copy);

    Circle_destroy(&copy);
    Shape_destroy(&copy.base);
    
    Circle_destroy(&c);
    Shape_destroy(&c.base);

    Circle_destroy(&unit);
    Shape_destroy(&unit.base);
    
    

    return 0;
}